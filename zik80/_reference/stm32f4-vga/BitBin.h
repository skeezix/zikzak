#ifndef __BITBIN_H__
#define __BITBIN_H__

#include "BitBinTables.h"

#include <stdint.h>
#include <stdbool.h>

#define MaximumBitBinChannels 8

typedef struct BitBinNote
{
	uint8_t note,waveform,volume,effect,parameter;
} BitBinNote;

typedef struct BitBinChannel
{
	int note;
	int waveform;
	int effect;
	int parameter;

	int currentvolume,requestedvolume,mastervolume;

	uint32_t phase,phasedelta;

	int vibrato,vibratophase;
	int portadest;

	uint8_t memory[13];

	BitBinNote *notes;
} BitBinChannel;

typedef struct BitBinSong
{
	uint32_t currentsample,nexttick,samplespertick;
	uint32_t currenttick,nextrow,ticksperrow;
	uint32_t currentrow,numrows;

	bool loops,stopped;

	const uint32_t *phasetable;

	int numchannels;
	BitBinChannel channels[MaximumBitBinChannels];
} BitBinSong;

void InitializeBitBinSong(BitBinSong *self,const uint32_t *phasetable,int numchannels,int numrows,BitBinNote **notes);
int16_t NextBitBinSample(BitBinSong *self);
void RenderBitBinSamples(BitBinSong *self,int numsamples,int16_t *samples);

static inline bool DoesBitBinSongLoop(BitBinSong *self) { return self->loops; }
static inline bool SetBitBinSongLoops(BitBinSong *self,bool loops) { self->loops=loops; }

static inline bool IsBitBinSongStopped(BitBinSong *self) { return self->stopped; }
static inline bool SetBitBinSongStopped(BitBinSong *self,bool stopped) { self->stopped=stopped; }

static inline int CurrentBitBinRow(BitBinSong *self) { return self->currentrow; }
static inline int CurrentBitBinTick(BitBinSong *self) { return self->currenttick; }

static inline BitBinNote BitBinNoteAtRow(BitBinSong *self,int channel,int row)
{
	return self->channels[channel].notes[row&127];
}

#endif
