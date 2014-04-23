#ifndef DRIVERARGUMENTS_H   /* -*- C++ -*- */
#define DRIVERARGUMENTS_H

#include "itype.h"
#include "Point.h"

#define DRIVERARGUMENTS_MAX_DISABLED_TILES 0xFF

namespace MFM {

  class DriverArguments
  {
  private:
    const char * m_programName;
    const char * m_dataDirPath;
    u32 m_seed;
    u32 m_haltAfterAEPS;
    u32 m_disabledTileCount;
    u32 m_aepsPerFrame;
    s32 m_recordEventCountsPerAEPS;
    s32 m_recordScreenshotPerAEPS;
    s32 m_recordTimeBasedDataPerAEPS;
    SPoint m_disabledTiles[DRIVERARGUMENTS_MAX_DISABLED_TILES];
    s32 m_countOfScreenshotsPerRate;
    bool m_startPaused;
    bool m_startWithoutGridView;
    bool m_startMinimal;
    u32 m_lastArgUsed;

  public:

    DriverArguments(int & argc, char ** &argv) :
      m_programName(0),
      m_dataDirPath(0),
      m_seed(0),
      m_haltAfterAEPS(0),
      m_disabledTileCount(0),
      m_aepsPerFrame(1),
      m_recordEventCountsPerAEPS(-1),
      m_recordScreenshotPerAEPS(-1),
      m_recordTimeBasedDataPerAEPS(-1),
      m_countOfScreenshotsPerRate(-1)
    {
      m_lastArgUsed = ProcessArguments(argc,argv);
    }

    void Die(const char * format, ...) __attribute__ ((noreturn));

    int ProcessArguments(int & argc, char ** &argv) ;

    const char * GetProgramName() const { return m_programName; }
    const char * GetDataDirPath() const { return m_dataDirPath; }
    u32 GetSeed() const { return m_seed; }
    u32 GetHaltAfterAEPS() const { return m_haltAfterAEPS; }
    u32 GetDisabledTileCount() const { return m_disabledTileCount; }
    u32 GetAEPSPerFrame() const { return m_aepsPerFrame; }
    s32 GetRecordEventCountsPerAEPS() const { return m_recordEventCountsPerAEPS; }
    s32 GetRecordScreenshotPerAEPS() const { return m_recordScreenshotPerAEPS; }
    s32 GetRecordTimeBasedDataPerAEPS() const { return m_recordTimeBasedDataPerAEPS; }
    SPoint* GetDisabledTiles() { return m_disabledTiles; }
    s32 GetCountOfScreenshotsPerRate() const { return m_countOfScreenshotsPerRate; }
    bool GetStartPaused() const { return m_startPaused; }
    bool GetStartWithoutGridView() const { return m_startWithoutGridView; }
    bool GetStartMinimal() const { return m_startMinimal; }
  };
} /* namespace MFM */

#endif /* DRIVERARGUMENTS_H */