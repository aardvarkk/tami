#ifndef FDSSOUND_H
#define FDSSOUND_H

void FDSSoundReset(void);
uint8 FDSSoundRead(uint16 address);
void FDSSoundWrite(uint16 address, uint8 value);
int32 FDSSoundRender(void);
void FDSSoundVolume(unsigned int volume);
void FDSSoundInstall3(void);

#endif /* FDSSOUND_H */
