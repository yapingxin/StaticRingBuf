#ifndef _INC_UNITTETST_TESTSTATICRINGBUF_VERIFY_TS_0001_H
#define _INC_UNITTETST_TESTSTATICRINGBUF_VERIFY_TS_0001_H

#define TS_0001_Identifier "TS_0001: StaticRingBuf"

#ifdef __cplusplus
extern "C" {
#endif

    int TS_0001_Setup(void);
    int TS_0001_Cleanup(void);

    void TC0001_STARB_Create_Release(void);
    void TC0002_STARB_Init_Release(void);

#ifdef __cplusplus
} // ! extern "C"
#endif

#endif // !_INC_UNITTETST_TESTSTATICRINGBUF_VERIFY_TS_0001_H
