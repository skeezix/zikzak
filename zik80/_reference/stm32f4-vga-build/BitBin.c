#include "BitBin.h"

#include <math.h>
#include <string.h>
#include <stdio.h>

static void InitializeBitBinChannel(BitBinChannel *channel,BitBinNote *notes);

static void MoveSongToNextSample(BitBinSong *self);
static void MoveSongToNextTick(BitBinSong *self);
static void MoveSongToNextRow(BitBinSong *self);

static void UpdateChannelAtRow(BitBinSong *self,BitBinChannel *channel);
static void UpdateChannelAtNonRowTick(BitBinSong *self,BitBinChannel *channel);
static void UpdateChannelAtSample(BitBinSong *self,BitBinChannel *channel);
static int32_t ChannelAmplitude(BitBinChannel *channel);
static int32_t SquareWave1_8(BitBinChannel *channel);
static int32_t SquareWave1_4(BitBinChannel *channel);
static int32_t SquareWave1_2(BitBinChannel *channel);
static int32_t SquareWave3_4(BitBinChannel *channel);
static int32_t TriangleWave(BitBinChannel *channel);
static int32_t Noise(BitBinChannel *channel);
static int32_t Drum1(BitBinChannel *channel);
static int32_t Drum2(BitBinChannel *channel);
static uint32_t Hash32(uint32_t val);

static uint32_t PhaseVelocityForNote(BitBinSong *self,int note);

static int8_t SineTable[48]=
{
	0,8,16,24,32,39,45,51,55,59,62,63,
	64,63,62,59,55,51,45,39,32,24,16,8,
	0,-8,-16,-24,-32,-39,-45,-51,-55,-59,-62,-63,
	-64,-63,-62,-59,-55,-51,-45,-39,-32,-24,-16,-8,
};

void InitializeBitBinSong(BitBinSong *self,const uint32_t *phasetable,int numchannels,int numrows,BitBinNote **notes)
{
	self->currentsample=0xffffffff;
	self->nexttick=0;
	self->samplespertick=phasetable[0]/50;

	self->currenttick=0xffffffff;
	self->nextrow=0;
	self->ticksperrow=3;

	self->currentrow=0xffffffff;
	self->numrows=numrows;

	self->loops=false;
	self->stopped=false;

	self->phasetable=&phasetable[1];

	self->numchannels=numchannels;
	for(int i=0;i<self->numchannels;i++) InitializeBitBinChannel(&self->channels[i],notes[i]);
}

static void InitializeBitBinChannel(BitBinChannel *channel,BitBinNote *notes)
{
	channel->note=0;
	channel->waveform=0;
	channel->effect=0;

	channel->phase=0;
	channel->phasedelta=0;

	channel->currentvolume=0;
	channel->requestedvolume=0;
	channel->mastervolume=64;

	memset(channel->memory,0,sizeof(channel->memory));

	channel->notes=notes;
}



int16_t NextBitBinSample(BitBinSong *self)
{
	if(self->stopped) return 0;

	MoveSongToNextSample(self);

	int32_t amplitude=0;
	for(int i=0;i<self->numchannels;i++)
	{
		amplitude+=ChannelAmplitude(&self->channels[i]);
	}

	amplitude>>=12+3;
	if(amplitude>32767) return 32767;
	else if(amplitude<-32768) return -32768;
	else return amplitude;
}

void RenderBitBinSamples(BitBinSong *self,int numsamples,int16_t *samples)
{
	for(int i=0;i<numsamples;i++) *samples++=NextBitBinSample(self);
}

static void MoveSongToNextSample(BitBinSong *self)
{
	self->currentsample++;
	if(self->currentsample==self->nexttick)
	{
		MoveSongToNextTick(self);
		self->nexttick+=self->samplespertick;
	}

	for(int i=0;i<self->numchannels;i++) UpdateChannelAtSample(self,&self->channels[i]);
}

static void MoveSongToNextTick(BitBinSong *self)
{
	self->currenttick++;
	if(self->currenttick==self->nextrow)
	{
		MoveSongToNextRow(self);
		self->nextrow+=self->ticksperrow;
		return;
	}

	for(int i=0;i<self->numchannels;i++) UpdateChannelAtNonRowTick(self,&self->channels[i]);
}

static void MoveSongToNextRow(BitBinSong *self)
{
	self->currentrow++;
	if(self->currentrow==self->numrows)
	{
		self->currentrow=0;
		if(!self->loops) self->stopped=true;
	}

	for(int i=0;i<self->numchannels;i++) UpdateChannelAtRow(self,&self->channels[i]);
}




static void UpdateChannelAtRow(BitBinSong *self,BitBinChannel *channel)
{
	int note=channel->notes[self->currentrow].note;
	int waveform=channel->notes[self->currentrow].waveform;
	int volume=channel->notes[self->currentrow].volume;
	int effect=channel->notes[self->currentrow].effect;
	int parameter=channel->notes[self->currentrow].parameter;

	if(note==0xfe)
	{
		channel->requestedvolume=0;
	}
	else
	{
		if(note!=0xff)
		{
			if(effect=='G') channel->portadest=note<<4;
			else channel->note=note<<4;
		}

		if(waveform!=0xff)
		{
			channel->waveform=waveform;
			channel->phase=0;
			channel->requestedvolume=64;
		}

		if(volume!=0xff)
		{
			channel->requestedvolume=volume;
		}

		if(effect!=0xff)
		{
			if(parameter==0) parameter=channel->memory[effect-'A'];
			else channel->memory[effect-'A']=parameter;
		}

		channel->effect=effect;
		channel->parameter=parameter;
	}

	// Always reset vibrato if the current effect is not vibrato.
	if(effect!='H')
	{
		channel->vibrato=0;
		channel->vibratophase=0;
	}

	int lo=parameter&0x0f;
	int hi=(parameter&0xf0)>>4;

	switch(channel->effect)
	{
		case 'A':
			self->ticksperrow=parameter;
		break;

		case 'D': 
			if(hi==0x0f)
			{
				if(lo==0) channel->requestedvolume+=15;
				else channel->requestedvolume-=lo;
			}
			else if(lo==0x0f)
			{
				if(hi==0) channel->requestedvolume-=15;
				else channel->requestedvolume+=hi;
			}

			if(channel->requestedvolume<0) channel->requestedvolume=0;
			if(channel->requestedvolume>64) channel->requestedvolume=64;
		break;

		case 'H':
			channel->vibrato=lo*SineTable[channel->vibratophase]>>6;
			channel->vibratophase+=hi;
			if(channel->vibratophase>=48) channel->vibratophase-=48;
		break;

		case 'J':
			channel->vibrato=0;
			channel->vibratophase=1;
		break;

		case 'M':
		   channel->mastervolume=parameter;
		break;
	}

	// Don't ramp volume on note starts.
	if(note!=0xff) channel->currentvolume=channel->requestedvolume;

	channel->phasedelta=PhaseVelocityForNote(self,channel->note+channel->vibrato);
}

static void UpdateChannelAtNonRowTick(BitBinSong *self,BitBinChannel *channel)
{
	if(channel->note==0xff || channel->effect==0xff) return;

	int parameter=channel->parameter;
	int lo=parameter&0x0f;
	int hi=(parameter&0xf0)>>4;

	switch(channel->effect)
	{
		case 'D': 
			if(hi==0) channel->requestedvolume-=lo;
			else if(lo==0) channel->requestedvolume+=hi;

			if(channel->requestedvolume<0) channel->requestedvolume=0;
			if(channel->requestedvolume>64) channel->requestedvolume=64;
		break;

		case 'E':
			channel->note-=parameter;
		break;

		case 'F':
			channel->note+=parameter;
		break;

		case 'G':
			if(channel->portadest>channel->note+parameter) channel->note+=parameter;
			else if(channel->portadest<channel->note-parameter) channel->note-=parameter;
			else channel->note=channel->portadest;
		break;

		case 'H':
			channel->vibrato=lo*SineTable[channel->vibratophase]>>6;
			channel->vibratophase+=hi;
			if(channel->vibratophase>=48) channel->vibratophase-=48;
		break;

		case 'J':
			switch(channel->vibratophase%3)
			{
				case 0: channel->vibrato=0; break;
				case 1: channel->vibrato=hi<<4; break;
				case 2: channel->vibrato=lo<<4; break;
			}
			channel->vibratophase++;
		break;
	}

	channel->phasedelta=PhaseVelocityForNote(self,channel->note+channel->vibrato);
}

static void UpdateChannelAtSample(BitBinSong *self,BitBinChannel *channel)
{
	if((self->currentsample&7)==0) // TODO: Should be sample rate dependent.
	{
		if(channel->currentvolume<channel->requestedvolume) channel->currentvolume++;
		else if(channel->currentvolume>channel->requestedvolume) channel->currentvolume--;
	}

	channel->phase+=channel->phasedelta;
}

static int32_t ChannelAmplitude(BitBinChannel *channel)
{
	if(channel->currentvolume==0) return 0;
	if(channel->mastervolume==0) return 0;

	int32_t sample=0;
	switch(channel->waveform)
	{
		case 0: sample=SquareWave1_8(channel); break;
		case 1: sample=SquareWave1_4(channel); break;
		case 2: sample=SquareWave1_2(channel); break;
		case 3: sample=SquareWave3_4(channel); break;
		case 4: sample=TriangleWave(channel); break;
		case 5: sample=Noise(channel); break;
		case 6: sample=Drum1(channel); break;
		case 7: sample=Drum2(channel); break;
	}

	return sample*channel->currentvolume*channel->mastervolume;
}

static int32_t SquareWave1_8(BitBinChannel *channel)
{
	if((channel->phase&0x70000)==0) return 32767;
	else return -32768;
}

static int32_t SquareWave1_4(BitBinChannel *channel)
{
	if((channel->phase&0x60000)==0) return 32767;
	else return -32768;
}

static int32_t SquareWave1_2(BitBinChannel *channel)
{
	if((channel->phase&0x40000)==0) return 32767;
	else return -32768;
}

static int32_t SquareWave3_4(BitBinChannel *channel)
{
	if((channel->phase&0x60000)!=0x60000) return 32767;
	else return -32768;
}

static int32_t TriangleWave(BitBinChannel *channel)
{
	uint32_t phase=channel->phase<<12;
	uint32_t trianglewave=((phase<<1)^((int32_t)phase)>>31);
	int32_t signedwave=trianglewave-0x80000000;
	return (signedwave>>16);
}

static int32_t Noise(BitBinChannel *channel)
{
	return Hash32(channel->phase>>16)&1?32767:-32768;
}

static int32_t Drum1(BitBinChannel *channel)
{
	static const int16_t samples[]=
	{
		-255,-27196,-32439,-21626,-13106,0,12779,24902,32439,32439,32439,32439,
		32439,30473,18021,14417,524,-12123,-23919,-32439,-32439,-32439,-32439,
		-32439,-32439,-32439,-32439,-32439,-32439,-32439,-32439,-27524,-19332,
		-6553,6553,19332,32767,32439,32439,32767,32439,32767,32767,32439,32439,
		32439,32439,29162,26541,19004,13434,5242,-6881,-20643,-32439,-32439,
		-32439,-32439,-32439,-32439,-32439,-32439,-32439,-32439,-32439,-32439,
		-30473,-25230,-21626,-8847,-1015,12779,25230,28834,32439,32439,32439,
		29817,31456,28834,32439,32439,28834,22609,21626,7536,-4587,-17366,-16711,
		-21626,-32439,-32439,-29817,-32439,-32439,-29817,-31128,-30473,-29817,
		-25885,-30145,-25230,-22281,-17038,-18021,-12779,-753,1015,8191,7536,
		14745,20970,24902,29817,32439,32439,29162,32439,32767,28834,32767,30145,
		32439,30800,29817,32111,28834,23264,23919,32767,28834,32767,26541,27196,
		30800,25230,21953,21298,20643,17694,13762,10485,8191,3080,-2817,-9830,
		-11140,-15728,-15728,-23919,-28834,-32439,-32439,-32439,-32439,-32439,
		-32439,-32439,-32439,-32439,-32439,-32439,-32439,-32439,-32439,-32439,
		-32439,-32439,-32439,-32439,-32439,-32439,-32439,-29817,-524,0
	};
	uint32_t index=channel->phase>>16;

	if(index>=sizeof(samples)/sizeof(samples[0])) return 0;
	else return samples[index]*2;
}

static int32_t Drum2(BitBinChannel *channel)
{
	static const int16_t samples[]=
	{
		32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,
		17038,15400,1802,-27524,-32439,-32439,-32439,-32439,-29817,-32439,-25558,
		20643,-17694,13762,32767,32767,10485,-5242,-15400,-24575,-32439,-20643,
		-25230,-18677,-29490,-16383,-19332,-21626,11140,8847,-21626,-32439,
		-21626,25558,13434,8847,4259,-255,32767,32767,32767,32767,27851,8519,
		14745,21626,18021,9830,-9502,-13434,-25230,6225,29490,-5570,3080,-20315,
		32767,22281,28507,-6225,-6553,-9502,-28507,-23592,-10485,-19332,-28507,
		-32439,-18677,255,19332,18021,21298,32767,32767,32767,32767,32767,32767,
		32767,17366,-25558,-32439,-32439,-32439,-32439,-32439,-32439,-32439,
		-32439,13434,32767,32767,32767,7208,32767,12451,20643,12123,-5570,-14417,
		-27851,-22609,-12779,-30473,-14745,-28834,-31456,-5242,-15728,-30473,
		-27524,-32439,-17038,255,-2555,255,255,15728,28834,32767,2293,18677,9830,
		-2555,-16383,-21626,-25230,-6225,-7536,-12123,-21298,4259,8847,-1277,
		12451,30473,-19332,1277,-3080,-7536,3604,17366,-8519,-15728,-18677,
		-19332,-32439,-9502,-32439,-31128,-11468,21298,32767,32111,22936,32767,
		15728,12123,27851,18021,20315,9830,-27851,-9830,-1277,-4587,1277,-27851,
		-16383,-1277,-5242,6225,9502,18021,-753,-9502,1802,-14417,753,-12779,
		-32439,-32439,-32439,-27524,-9830,-7536,-14745,8519,-32439,9830,6553,
		-3080,-13434,753,-16383,14417,7864,22936,32767,32767,32767,32767,32767,
		13434,23919,18021,753,-32439,-16383,-32439,-32439,-18677,-32439,-18677,
		-32439,-32439,-32439,-24575,-8519,-14417,-27851,-22936,-5570,-14417,8519,
		-12123,29490,7864,32767,7208,32767,26213,23592,32767,-7536,13434,9830,
		29817,28507,18677,20643,-753,13762,-8847,9502,-23592,-13434,-16383,
		-18021,-27851,-20643,-1802,-21298,-3080,-3080,3080,-32111,-10485,-22609,
		2293,4259,-4259,-3080,15400,-1277,21626,13434,14417,22936,25558,32767,
		18021,23592,25558,19332,-3604,-255,22281,8847,3080,11140,12123,32767,
		23592,-9502,1802,-23919,-15728,-16383,-22609,-32439,-17694,-13762,-23592,
		-26213,-27851,-2555,2293,255,7864,11468,-5242,-4587,-8847,7864,19660,
		-6225,21626,15400,10485,21298,27851,12451,17038,-10485,10485,7208,11468,
		12451,4259,22281,21626,27851,-10485,-9830,-23919,-32439,753,-25558,
		-13762,-25230,-21626,-13434,-26213,-2555,-5570,-13762,5242,-2555,-13762,
		6225,14417,15728,17038,28834,13434,28507,9502,27196,-5242,255,6225,-6553,
		18677,5242,4587,5242,-1277,7864,2293,4259,-11140,29490,8519,9830,6553,
		-6553,-1802,-753,-6225,-32439,-32439,-23592,-32439,-32439,-20643,-13434,
		-4259,5242,29817,26541,32767,32767,23919,27851,17038,20643,9502,-18677,
		-8519,753,11140,753,9502,-1802,-3604,12123,5242,-753,5242,-4587,-9502,
		1802,-4587,-12779,-6225,-12779,-15400,-14417,-6225,-2555,-29490,-18021,
		-7536,-22936,6225,-6225,-6553,13434,11468,21626,32767,32767,28507,32767,
		32767,32767,18677,-255,14417,-3080,-7536,3080,-17038,-27851,-23919,
		-27851,-14417,2293,-13434,-7864,-11468,-17038,-22609,-11140,-20643,-12779,
		-5242,-5242,753,5242,1277,10485,255,1802,5570,23919,12123,28507,9502,
		31456,19332,9830,32767,30473,7208,4259,9830,5242,-8847,-13434,-16383,
		-11468,1277,-23592,-8847,-20643,-15400,-5242,-11140,-11468,-8519,-2555,-
		7536,4259,3604,6225,2293,17366,2293,32767,26541,26541,8519,7208,4587,
		-753,-4259,-12123,-9502,-7864,-1277,-17038,-5242,7208,4587,4259,9830,3604,
		5570,18021,-1277,6225,0
	};
	uint32_t index=channel->phase>>16;

	if(index>=sizeof(samples)/sizeof(samples[0])) return 0;
	else return samples[index]*2;
}

static uint32_t Hash32(uint32_t val)
{
	val^=val>>16;
	val^=61;
	val+=val<<3;
	val^=val>>4;
	val*=0x27d4eb2d;
	val^=val>>15;
	return val;
}

static uint32_t PhaseVelocityForNote(BitBinSong *self,int note)
{
	unsigned int octave=(unsigned int)note/(12*16);
	unsigned int index=(unsigned int)note%(12*16);

	return self->phasetable[index]>>(10-octave);
}
