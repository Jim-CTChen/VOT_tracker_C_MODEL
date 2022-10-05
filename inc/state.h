#ifndef __STATE_H__
#define __STATE_H__

//////////// main state ////////////
const int S_IDLE = 0;
const int S_INIT = 1;
const int S_TRACK = 2;
const int S_END = 3;

//////////// sub state ////////////
// compress input image
const int SUB_S_COMPRESS_INPUT = 0;

// backbone alexnet
const int SUB_S_ALEX_CONV1 = 1;
const int SUB_S_ALEX_CONV2_DW = 2;
const int SUB_S_ALEX_CONV2_PW = 3;
const int SUB_S_MAXPOOL1 = 4;
const int SUB_S_ALEX_CONV3_DW = 5;
const int SUB_S_ALEX_CONV3_PW = 6;
const int SUB_S_MAXPOOL2 = 7;
const int SUB_S_ALEX_CONV4_DW = 8;
const int SUB_S_ALEX_CONV4_PW = 9;
const int SUB_S_ALEX_CONV5_DW = 10;
const int SUB_S_ALEX_CONV5_PW = 11;
const int SUB_S_ALEX_CONV6_DW = 12;
const int SUB_S_ALEX_CONV6_PW = 13;

// split two branch
const int SUB_S_CZK = 8;
const int SUB_S_RZK = 9;

// classification branch
const int SUB_S_C_DXCORR = 10;
const int SUB_S_C_CONV1 = 11;
const int SUB_S_C_CONV2 = 12;
const int SUB_S_C_CONV3 = 13;
const int SUB_S_C_CLS_CONV = 14;
const int SUB_S_C_CTR_CONV = 15;
const int SUB_S_C_FINAL = 16;

// regression branch
const int SUB_S_R_DXCORR = 17;
const int SUB_S_R_CONV1 = 18;
const int SUB_S_R_CONV2 = 19;
const int SUB_S_R_CONV3 = 20;
const int SUB_S_R_REG_CONV = 21;


#endif // __STATE_H__