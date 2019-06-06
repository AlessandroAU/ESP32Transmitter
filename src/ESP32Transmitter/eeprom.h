#pragma once

enum ADCfilter_ {LPF_20Hz, LPF_50Hz, LPF_100Hz, LPF_150Hz};

ADCfilter_ ADCfilter;

uint16_t CalDataADCminValues[6];
uint16_t CalDataADCmaxValues[6];
