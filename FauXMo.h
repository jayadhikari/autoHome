#ifndef _FAUXMO_H_
#define _FAUXMO_H_

typedef union deviceflags
{
    struct
    {
        unsigned DrawingRoomTubeFlag:1;
        unsigned DrawingRoomBulbFlag:1;
        unsigned BedRoomTubeFlag:1;
        unsigned BedRoomBulbFlag:1;
        unsigned fanFlag:1;
        unsigned fancyLightflag:1;
        unsigned reserved1:1;
        unsigned reserved2:1;                
      }bits;
     unsigned char Byte;
  
}DevFlags;
typedef union deviceState
{
    struct
    {
        unsigned DrawingRoomTubeFlag:1;
        unsigned DrawingRoomBulbFlag:1;
        unsigned BedRoomTubeFlag:1;
        unsigned BedRoomBulbFlag:1;
        unsigned fanFlag:1;
        unsigned fancyLightflag:1;
        unsigned reserved1:1;
        unsigned reserved2:1;                
      }bits;
     unsigned char Byte;
  
}DevState;


void handleFauxmp(void);
void fauxmoInit(void);
void addSmartDevices(void);
void checkActionTobePerformed(void);

#endif
