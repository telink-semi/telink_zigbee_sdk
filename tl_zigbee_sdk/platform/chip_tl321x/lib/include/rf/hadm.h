/********************************************************************************************************
 * @file    hadm.h
 *
 * @brief   This is the header file for TL321X
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#define HADM_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))
#define HADM_VERSION                          HADM_VERSION_INT(1, 1, 17)

    //#define STD_GCC //standard open source risc-V GCC

    //#define INITIATOR 0
    //#define REFLECTOR 1

#define CHANNUM      (40)

#define MAXOSR       (4)
#define PI           (3.141592653589793)
#define SPEEDOFLIGHT (299792458)
#define SAMPLERATE   (4e6)
#define FCLK         (24e6)
#define FIF          (1e6)

//fcs
#define MAXFCSLEN (100 * MAXOSR) //80us in standard

//tesCollectData
#define MAXTESLEN (40 * MAXOSR) // 40/20/10 us in standard
#define CALIFACOR 10

//pesCollectData
#define MAXAALEN    (32) //4 oct in standard
#define MAXPESIQLEN (240)
#define FTEPREC     (2048)
#define FTEPRECINV  (0.00048828125)

//tesCalcDistance
#define MAXCHANNUM  80
#define MAXMATLEN   40
#define MAXHMATLEN  (((MAXMATLEN + 1) * MAXMATLEN) / 2) //length of hermitiann matrix
#define MAXNUMSEP   (MAXCHANNUM / 10)
#define SATTHRESPOS (524287)
#define SATTHRESNEG (-524288)

//pesCalcDistance
#define DISTSYNCOFFSET (18398.16581556574)
#define DISTOFFSET     (6780.455355661499)
#define DISTP          (7.51)


#define FLOATPOINT     0

#if FLOATPOINT
    #define PRECBITS         0
    #define PRECBITSNORM     1 //2^22
    #define PRECBITSSQRT     0
    #define PRECBITSSQRTNORM 1
    #define DIGITTYPE        float
    #define DIGITTYPELONG    float
    #define DIGITTYPESHORT   float
    #define TOL1             1e-7
#else
    #define PRECBITS         28
    #define PRECBITSNORM     268435456 //2^PRECBITS
    #define PRECBITSSQRT     14
    #define PRECBITSSQRTNORM 16384
    #define PRECQ15NORM      32767
    #define TOL1             (1e-7 * PRECBITSNORM)

    #ifndef STD_GCC //standard open source risc-V GCC
        #include <nds_math_types.h>
        #include <nds_utils_math.h>
        #include <nds_basic_math.h>
        #include <nds_statistics_math.h>
        #include <nds_complex_math.h>

        //#define DIGITTYPE q31_t
        #define DIGITTYPE      int
        #define DIGITTYPELONG  q63_t
        #define DIGITTYPESHORT q15_t
    #else
        //#define DIGITTYPE __int32
        //#define DIGITTYPELONG __int64
        //#define DIGITTYPESHORT __int16
        #define DIGITTYPE      __INT32_TYPE__
        #define DIGITTYPELONG  __INT64_TYPE__
        #define DIGITTYPESHORT __INT16_TYPE__

    #endif
#endif

    //Define the IQ data type.
    struct complex
    {
        DIGITTYPE real;
        DIGITTYPE imag;
    };
    typedef struct complex complex;

    //It is used for the collection of IQ data at both ends of the tes phase.
    struct tesInfo
    {
        float   distanceEst;  //for simulation only
        float   distanceReal; //for simulation only
        float   likeliness;
        float   RSSI_initiator;
        float   RSSI_reflector;
        complex I[MAXCHANNUM]; // iq data collected from initiator
        complex Q[MAXCHANNUM]; // iq data collected from reflector
    };
    typedef struct tesInfo tesInfo;

    //Receive the return value of the TES initialization stage and provide it to the
    //algorithm for distance calculation.
    struct parameterConstTes
    {
        float     cPhase2R;
        float     firstPeakTolx;
        DIGITTYPE invDen;
        DIGITTYPE firstPeakMinLevel;
        float     tol2;
        float     invPREC;
        float     invPRECSQRT;
        float     l2s2R;
        float     x[MAXCHANNUM];
        float     fstep;
        int       channum;
        int       matlen;
        int       hmatlen;
        int       numsep;
        int       meandiv;
        int       meandivnorm;
    };
    typedef struct parameterConstTes parameterConstTes;

    //Used to record RSSI and FTE information on both ends.
    struct pesInfo
    {
        float RSSI_initiator;
        float RSSI_reflector;
        int   fteInitiator;
        int   fteReflector;
    };
    typedef struct pesInfo pesInfo;

    //Receive the return value of the PES initialization stage and provide it to the
    //algorithm for distance calculation.
    struct parameterConstPes
    {
        int   n;
        int   role;
        float distSyncOffset;
        float distCoarseOffset;
        float distOffset;
        float distP;
        float distJump;
        float xTick;
        int   osr;
        int   aaCode[MAXAALEN];
        float aaSeqIQ[MAXAALEN * 2];
        int   aaCodeLen;
        int   offset;
    };
    typedef struct parameterConstPes parameterConstPes;


    /*
 * @brief           This function is mainly used to return the version number of the algorithm library.
 * @param[in]       none.
 * @return          The version of hadm.a.
 */
    int get_version();

    /*
 * @brief           This function is mainly used to estimate the frequency offset information through IQ data.
 * @param[in]       IQData  -   IQ data collected during the FCS phase.
 * @param[in]       IQLen   -   The number of groups of IQ data collected during the FCS phase.
 * @param[in]       cfoCoarse   -   coarse CFO(carrier frequency offset)estimation value.
 * @param[in]       sampleRate  -   Sampling frequency of the FCS stage.
 * @return          The frequency offset between initiator and reflector.
 */
    float calcFreq(float IQData[], int IQLen, float cfoCoarse, float sampleRate);

    //pesCollectData
    /*
 * @brief           This function is mainly used to set the frequency offset information from mode0 for the algorithm in mode1 calculation.
 * @param[in]       cfo         -   Frequency offset result of mode0 output.
 * @param[out]      aaSeqIqCfo  -   Used to store the bit sequence of the access code with the frequency bias processed information.
 * @param[in]       para        -   Output from pesInit.
 * @return          0:success; others:fail.
 */
    int pesSetCfo(float cfo, float aaSeqIqCfo[], parameterConstPes para);

    /*
 * @brief           This function is mainly used to calculate cte, fte, etc. by IQ and timestamp.
 * @param[in]       tx_timestamp    -   The timestamp value at the moment tx_en is pulled high during each send.
 * @param[in]       iq_start_tstamp -   Timestamp of each time the rx side starts collecting IQ data.
 * @param[in]       aaSeqIqCfo      -   Information on the combination of access code and frequency bias calculated by pesSetCfo.
 * @param[in]       iqData          -   IQ data acquired during packet interactions.
 * @param[in]       IQLen           -   The number of groups of IQ data collected during the each packet interactions.
 * @param[out]      cte             -   Used to store calculated cte information.
 * @param[out]      fte             -   Used to store calculated fte information.
 * @param[in]       para            -   Output from pesInit.
 * @return          0:success; others:false.
 */
    int calcPesInfoIQ(int tx_timestamp, int iq_start_tstamp, float aaSeqIqCfo[], int iqData[], int IQLen, short *cte, short *fte, parameterConstPes para);

    //pesCalcDistance
    /*
 * @brief           This function is mainly used for distance calculation via packet interaction.
 * @param[in]       cte_sync1   -   Timestamp information processed on the initiator side by the calcPesInfo function.
 * @param[in]       cte_sync2   -   Timestamp information processed on the reflector side by the calcPesInfo function.
 * @param[in]       sync_flag   -   Identifies whether both ends of the interaction sync to the packet from the other end.1:success,0:fail.
 * @param[in]       distSync1   -   Used to store temporary data during calculation.
 * @param[in]       para        -   Output from pesInit.
 * @return          Distances estimated from packet interaction information.
 */
    float pesCalcDistSync(short cte_sync1[], short cte_sync2[], int sync_flag[], float distSync1[], parameterConstPes para);

    /*
 * @brief           This function is mainly used for distance calculation via packet interaction.
 * @param[in]       distPesSync -   Distance results output by pesCalcDistSync.
 * @param[in]       cte1        -   Cte information derived from calcPesInfoIQ on initiator side.
 * @param[in]       cte2        -   Cte information derived from calcPesInfoIQ on reflector side.
 * @param[in]       fte1        -   Fte information derived from calcPesInfoIQ on initiator side.
 * @param[in]       fte2        -   Fte information derived from calcPesInfoIQ on reflector side.
 * @param[in]       sync_flag   -   Identifies whether both ends of the interaction sync to the packet from the other end.1:success,0:fail.
 * @param[out]      distPes1    -   For storing intermediate variables.
 * @param[in]       para        -   Output from pesInit.
 * @return          Distances estimated from packet interaction information.
 */
    float pesCalcDist(float distPesSync, short cte1[], short cte2[], short fte1[], short fte2[], int sync_flag[], float distPes1[], parameterConstPes para);

    /*
 * @brief           This function is mainly used to calculate the information of Pes.
 * @param[in]       tx_timestamp    -   The timestamp of the tx_on moment.
 * @param[in]       sync_timestamp  -   Timestamp when synchronizing to packets.
 * @param[out]      cte_sync        -   Used to store cte_sync information during arithmetic operations.
 * @param[in]       para            -   Output from pesInit.
 * @return          0:success; others:false.
 */
    int calcPesInfo(int tx_timestamp[], int sync_timestamp[], short cte_sync[], parameterConstPes para);

    /*
 * @brief           This function is mainly used for initializing the distance calculation algorithm in PES mode.
 * @param[in]       nAverage        -   Number of packet interactions performed.
 * @param[in]       role            -   Chip as initiator or reflector.
 * @param[in]       dataRate        -   Chip operates in 1M PHY or 2M PHY.
 * @param[in]       aaCode          -   Value of access code.
 * @param[in]       aaCodeLen       -   Length of access code.Unit is bit.
 * @param[in]       offset          -   Use the starting point of the IQ data, i.e. which IQ to start the calculation from.
 * @return          Structures after initialization.
 */
    parameterConstPes pesInit(int nAverage, int role, int dataRate, int aaCode[], int aaCodeLen, int offset);

    /*
 * @brief           This function is mainly used to calculate the compensation value.
 * @param[in]       compArr         -   The address where the compensation value is stored.
 * @param[in]       IQLen           -   The number of groups of IQ data collected.
 * @param[in]       angleStep       -   The step value of the angle.
 * @return          0:success; others:false.
 */
    int calcCompensate(float compArr[], int IQLen, float angleStep);

    /*
 * @brief           This function is mainly used to calculate the information of Tes.
 * @param[in]       IQData          -   The address where the compensation value is stored.
 * @param[in]       compArr         -   The number of groups of IQ data collected.
 * @param[in]       IQLen           -   The step value of the angle.
 * @param[in]       iq_timeStamp    -   The timestamp when the IQ data collection begins.
 * @param[in]       trx_timeStamp   -   TX RX conversion time.
 * @param[in]       cfo             -   The frequency offset at both ends of initiator and reflect, plus and minus signs are determined by the character.
 * @param[in]       if_adjustment   -   Frequency compensation value.
 * @param[in]       output          -   Processed IQ data.
 * @return          0:success; others:false.
 */
    int calcTesInfo(int IQData[], const float compArr[], int IQLen, int iq_timeStamp, int trx_timeStamp, float cfo, float if_adjustment, int output[], const float thresGood, const float thresBad);

    /*
 * @brief           This function is mainly used to compress IQ data collected during the tes phase.
 * @param[in]       ipm     -   IQ data processed by calcTesInfo.
 * @param[in]       IQLen   -   The number of groups of IQ data collected.
 * @return          0:success; others:false.
 */
    int compressTesInfo(int ipm[], int len);

    /*
 * @brief           This function is mainly used for initializing the distance calculation algorithm in TES mode.
 * @param[in]       none.
 * @return          The parameter of distance calculate.
 */
    parameterConstTes tesInit(int channum, float fstep);

    /*
 * @brief           This function is mainly used to calculate the compensation value.
 * @param[in]       I[]     -   IQ data processed by initiator.
 * @param[in]       Q[]     -   IQ data processed by reflector.
 * @param[in]       cali    -   Calibration compensation.
 * @param[in]       H2WR[]  -   The address where the results of the operation are stored.
 * @return          0:success; others:false.
 */
    int calcIpmPct(const int I[], const int Q[], const int cali[], complex H2WR[], parameterConstTes para);

    /*
 * @brief           This function is used to estimate the distance by means of phase difference.
 * @param[in]       H2WR[]  -   IQ data processed by calcIpmPct.
 * @param[in]       T2WR[]  -   Intermediate calculation results.
 * @param[in]       T2WRDiffMean    -   Intermediate calculation results.
 * @param[in]       para    -   The results produced by tesInit.
 * @return          The result of distance.
 */
    float tesPhase(const complex H2WR[], float T2WR[], float *T2WRDiffMean, parameterConstTes para);

    /*
 * @brief           This function is used to estimate the distance by means of phase difference.
 * @param[in]       H2WR[]  -   IQ data processed by calcIpmPct.
 * @param[in]       T2WR[]  -   Intermediate calculation results.
 * @param[in]       T2WRDiffMean    -   Intermediate calculation results.
 * @param[in]       para    -   The results produced by tesInit.
 * @param[in]       likeliness -    Variables for debugging.
 * @param[in]       nIterMaxEig -   Variables for debugging.
 * @param[in]       nIterPS     -   Intermediate calculation results.
 * @param[in]       nSigCnt     -   Intermediate calculation results.
 * param[in]        EVDCap      -   Intermediate calculation results.
 * @return          The result of distance.
 */
    float tesMusic(complex H2WR[], float T2WR[], float T2WRDiffMean, parameterConstTes para, float *likeliness, float *nIterMaxEig, float *nIterPS, float *nSigCnt, float *EVDCap);
#ifdef __cplusplus
}
#endif
