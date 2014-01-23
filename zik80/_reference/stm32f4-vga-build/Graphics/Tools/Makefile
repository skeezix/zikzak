CC = gcc

ALL_CFLAGS =	$(CFLAGS) \
		-std=c99

LIBS = -lm

EXECUTABLE_FILES =	CropPNG \
			MixPNGChannels \
			PNGToAdaptiveBitmapSource \
			PNGToBitmapSource \
			PNGToRLEBitmapSource \
			QuantizePNG \
			RemapPNG

DEPENDENCY_DIR = Dependencies

AUTODEPENDENCY_FLAGS = -MMD -MF$(@:%=$(DEPENDENCY_DIR)/%.d) -MT$@

all: $(EXECUTABLE_FILES)

CropPNG: CropPNG.c
	@mkdir -p $(DEPENDENCY_DIR)
	$(CC) $(ALL_CFLAGS) $(AUTODEPENDENCY_FLAGS) -o $@ $< $(LIBS)

MixPNGChannels: MixPNGChannels.c
	@mkdir -p $(DEPENDENCY_DIR)
	$(CC) $(ALL_CFLAGS) $(AUTODEPENDENCY_FLAGS) -o $@ $< $(LIBS)

PNGToAdaptiveBitmapSource: PNGToAdaptiveBitmapSource.c
	@mkdir -p $(DEPENDENCY_DIR)
	$(CC) $(ALL_CFLAGS) $(AUTODEPENDENCY_FLAGS) -o $@ $< $(LIBS)

PNGToBitmapSource: PNGToBitmapSource.c
	@mkdir -p $(DEPENDENCY_DIR)
	$(CC) $(ALL_CFLAGS) $(AUTODEPENDENCY_FLAGS) -o $@ $< $(LIBS)

PNGToRLEBitmapSource: PNGToRLEBitmapSource.c
	@mkdir -p $(DEPENDENCY_DIR)
	$(CC) $(ALL_CFLAGS) $(AUTODEPENDENCY_FLAGS) -o $@ $< $(LIBS)

QuantizePNG: QuantizePNG.c
	@mkdir -p $(DEPENDENCY_DIR)
	$(CC) $(ALL_CFLAGS) $(AUTODEPENDENCY_FLAGS) -o $@ $< $(LIBS)

RemapPNG: RemapPNG.c
	@mkdir -p $(DEPENDENCY_DIR)
	$(CC) $(ALL_CFLAGS) $(AUTODEPENDENCY_FLAGS) -o $@ $< $(LIBS)

clean:
	rm -rf $(EXECUTABLE_FILES) $(DEPENDENCY_DIR)

-include $(DEPENDENCY_DIR)/*.d
