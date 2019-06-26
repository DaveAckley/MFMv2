/* -*- C++ -*- */
#ifndef ITCSTATUS_H
#define ITCSTATUS_H

#include "itype.h"
#include "ByteSource.h"
#include "Point.h"
#include "Drawing.h"

#include "ITCIcons.h"
#include "T2VizConstants.h"

namespace MFM {

  struct ITCStatus {

    static const char * (dirNames[ITC_COUNT]);
    static const u32 (iconFaceIdx[ITC_COUNT]);
    static const UPoint (screenPos[ITC_COUNT]);
    static const SPoint (side1Pos[ITC_COUNT]);
    static const SPoint (side2Pos[ITC_COUNT]);

    const char * getAbbr() const ;

    enum {
      BYTES_SENT_IDX = 0,
      BYTES_RCVD_IDX,
      PACKETS_SENT_IDX,
      PACKETS_RCVD_IDX,
      PACKET_SYNCS_IDX,
      SYNC_FAILS_IDX,
      TIMEOUTS_IDX,
      ITC_STAT_COUNT
    };
    u32 mDirIdx; // 0..5
    bool mIsOpen;
    u32 mStats[ITC_STAT_COUNT];
    u32 mDeltas[ITC_STAT_COUNT];

    const char * getDirName() const { return dirNames[mDirIdx]; }
    const u32 getIconFaceIdx() const { return iconFaceIdx[mDirIdx]; }
    const UPoint getScreenPos() const { return screenPos[mDirIdx]; }
    const SPoint getSide1Offset() const { return side1Pos[mDirIdx]; }
    const SPoint getSide2Offset() const { return side2Pos[mDirIdx]; }
    const bool isVertical() const { return mDirIdx == 1 || mDirIdx == 4; }

    u32 mapBytesToSizeIdx(u32 bytes) ;

    void drawIconAt(Drawing & draw, ITCIcons& icons, SPoint offset, u32 funcIdx, u32 sizeIdx) ;

    ITCStatus() ;

    void setDirIdx(u32 dirIdx) ;

    bool isOpen() { return mIsOpen; }
    void setIsOpen(bool newopen) { mIsOpen = newopen; }

    void updateStatsFromLine(ByteSource & bs) ;

    void formatDeltasLine(ByteSink & bs) ;

    void drawStatus(Drawing & draw, ITCIcons & icons) ;
      
  };
}

#endif /* ITCSTATUS_H */