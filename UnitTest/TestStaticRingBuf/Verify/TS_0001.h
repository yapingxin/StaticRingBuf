#ifndef _INC_UNITTETST_TESTSTATICRINGBUF_VERIFY_TS_0001_H
#define _INC_UNITTETST_TESTSTATICRINGBUF_VERIFY_TS_0001_H

#define TS_0001_Identifier "TS_0001: StaticRingBuf"

#ifdef __cplusplus
extern "C" {
#endif

    int TS_0001_Setup(void);
    int TS_0001_Cleanup(void);

    void TC0002_STARB_Init_Release(void);
    void TC0003_STARB_Write1Byte(void);
    void TC0004_STARB_TestCapBound_Write(void);
    void TC0005_STARB_WriteItems(void);
    void TC0006_STARB_TestCapBound_WriteItems(void);

#ifdef __cplusplus
} // ! extern "C"
#endif

#endif // !_INC_UNITTETST_TESTSTATICRINGBUF_VERIFY_TS_0001_H
