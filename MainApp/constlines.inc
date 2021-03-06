const struct ConstellationLine ConstellationLineList[] = {
    {"AND",  17.415,  35.61,  30.975,  42.33},
    {"AND",   2.100,  29.08,   9.825,  30.86},
    {"AND", 345.465,  42.33, 354.390,  46.44},
    {"AND", 354.390,  46.44,  14.190,  38.50},
    {"AND",   9.225,  33.71,   2.100,  29.08},
    {"AND",   9.825,  30.86,  17.415,  35.61},
    {"AND",  14.190,  38.50,  17.370,  47.25},
    {"AND",  14.190,  38.50,   9.225,  33.71},
    {"ANT", 156.795, -31.06, 164.175, -37.13},
    {"ANT", 156.795, -31.06, 142.290, -35.95},
    {"APS", 221.970, -79.05, 246.720, -78.73},
    {"APS", 245.070, -78.70, 248.370, -78.89},
    {"APS", 245.070, -78.70, 250.770, -77.51},
    {"APS", 248.370, -78.89, 250.770, -77.51},
    {"AQL", 284.895,  15.06, 286.350,  13.86},
    {"AQL", 285.420,  -5.73, 286.545,  -4.88},
    {"AQL", 286.350,  13.86, 296.565,  10.61},
    {"AQL", 286.545,  -4.88, 291.375,   3.11},
    {"AQL", 291.375,   3.11, 296.565,  10.61},
    {"AQL", 294.165,  -1.28, 286.545,  -4.88},
    {"AQL", 296.565,  10.61, 297.690,   8.86},
    {"AQL", 297.690,   8.86, 298.815,   6.40},
    {"AQL", 298.110,   1.00, 294.165,  -1.28},
    {"AQL", 298.815,   6.40, 302.820,  -0.81},
    {"AQL", 302.820,  -0.81, 298.110,   1.00},
    {"AQR", 311.910,  -9.49, 322.890,  -5.56},
    {"AQR", 322.890,  -5.56, 330.825,  -2.15},
    {"AQR", 330.825,  -2.15, 335.415,  -1.38},
    {"AQR", 335.415,  -1.38, 337.185,  -0.01},
    {"AQR", 337.185,  -0.01, 336.315,   1.38},
    {"AQR", 337.185,  -0.01, 338.850,  -0.11},
    {"AQR", 337.185,  -0.01, 343.140,  -7.58},
    {"AQR", 342.390, -13.59, 343.650, -15.81},
    {"AQR", 343.140,  -7.58, 342.390, -13.59},
    {"AQR", 343.650, -15.81, 347.340, -21.16},
    {"AQR", 347.340, -21.16, 351.495, -20.64},
    {"AQR", 348.570,  -6.05, 343.140,  -7.58},
    {"AQR", 349.740,  -9.61, 348.570,  -6.05},
    {"AQR", 351.495, -20.64, 356.490, -18.68},
    {"AQR", 355.665, -14.54, 349.740,  -9.61},
    {"AQR", 356.490, -18.68, 355.665, -14.54},
    {"ARA", 254.640, -55.98, 252.450, -59.03},
    {"ARA", 254.640, -55.98, 254.895, -53.16},
    {"ARA", 261.315, -55.53, 254.640, -55.98},
    {"ARA", 261.315, -55.53, 261.345, -56.38},
    {"ARA", 261.345, -56.38, 262.770, -60.68},
    {"ARA", 262.935, -49.88, 261.315, -55.53},
    {"ARA", 271.635, -50.10, 262.935, -49.88},
    {"ARI",  28.365,  19.29,  28.650,  20.79},
    {"ARI",  28.650,  20.79,  31.785,  23.46},
    {"ARI",  31.785,  23.46,  47.895,  19.73},
    {"AUR",  74.235,  33.16,  76.620,  41.23},
    {"AUR",  76.620,  41.23,  79.170,  45.99},
    {"AUR",  79.170,  45.99,  89.865,  54.28},
    {"AUR",  81.570,  28.60,  74.235,  33.16},
    {"AUR",  89.865,  44.95,  79.170,  45.99},
    {"AUR",  89.865,  44.95,  89.910,  37.21},
    {"AUR",  89.865,  54.28,  89.970,  45.93},
    {"AUR",  89.910,  37.21,  81.570,  28.60},
    {"AUR",  89.970,  45.93,  89.865,  44.95},
    {"BOO", 208.665,  18.39, 207.375,  15.79},
    {"BOO", 213.915,  19.18, 208.665,  18.39},
    {"BOO", 213.915,  19.18, 217.935,  30.36},
    {"BOO", 213.915,  19.18, 220.260,  13.73},
    {"BOO", 214.095,  46.08, 216.300,  51.84},
    {"BOO", 217.935,  30.36, 218.010,  38.30},
    {"BOO", 218.010,  38.30, 214.095,  46.08},
    {"BOO", 218.010,  38.30, 225.465,  40.38},
    {"BOO", 221.250,  27.06, 213.915,  19.18},
    {"BOO", 225.465,  40.38, 228.870,  33.31},
    {"BOO", 228.870,  33.31, 221.250,  27.06},
    {"CAE",  67.695, -44.94,  70.140, -41.86},
    {"CAE",  70.140, -41.86,  70.515, -37.14},
    {"CAE",  70.515, -37.14,  76.095, -35.48},
    {"CAM",  57.600,  71.33,  73.485,  66.35},
    {"CAM",  73.485,  66.35,  75.840,  60.44},
    {"CAM",  73.485,  66.35,  93.195,  65.71},
    {"CAM",  75.840,  60.44,  74.310,  53.75},
    {"CAM",  93.195,  65.71, 103.425,  68.88},
    {"CAP", 304.395, -12.49, 305.235, -14.78},
    {"CAP", 305.235, -14.78, 311.520, -25.26},
    {"CAP", 311.520, -25.26, 312.945, -26.91},
    {"CAP", 312.945, -26.91, 316.770, -25.00},
    {"CAP", 316.470, -17.23, 304.515, -12.54},
    {"CAP", 316.770, -25.00, 321.660, -22.41},
    {"CAP", 320.550, -16.83, 316.470, -17.23},
    {"CAP", 321.660, -22.41, 324.270, -19.46},
    {"CAP", 324.270, -19.46, 326.745, -16.13},
    {"CAP", 325.020, -16.66, 320.550, -16.83},
    {"CAP", 326.745, -16.13, 325.020, -16.66},
    {"CAR", 153.420, -70.03, 138.285, -69.71},
    {"CAR", 156.840, -57.63, 157.995, -61.68},
    {"CAR", 157.995, -61.68, 160.740, -64.40},
    {"CAR", 160.740, -64.40, 153.420, -70.03},
    {"CAR",  95.985, -52.70, 119.190, -52.98},
    {"CAR", 119.190, -52.98, 125.610, -59.51},
    {"CAR", 125.610, -59.51, 137.745, -58.96},
    {"CAR", 125.610, -59.51, 140.520, -55.01},
    {"CAR", 138.285, -69.71, 146.775, -65.06},
    {"CAR", 139.275, -59.28, 156.840, -57.63},
    {"CAR", 146.775, -65.06, 139.275, -59.28},
    {"CAS",  21.450,  60.23,  28.590,  63.66},
    {"CAS",   2.295,  59.14,  10.125,  56.53},
    {"CAS",  10.125,  56.53,  14.160,  60.71},
    {"CAS",  14.160,  60.71,  21.450,  60.23},
    {"CEN", 170.250, -54.48, 182.100, -50.71},
    {"CEN", 173.940, -63.01, 170.250, -54.48},
    {"CEN", 182.100, -50.71, 190.365, -48.96},
    {"CEN", 190.365, -48.96, 200.145, -36.71},
    {"CEN", 200.145, -36.71, 211.665, -36.36},
    {"CEN", 204.960, -53.46, 190.365, -48.96},
    {"CEN", 204.960, -53.46, 210.945, -60.36},
    {"CEN", 208.860, -47.28, 204.960, -53.46},
    {"CEN", 210.945, -60.36, 219.885, -60.83},
    {"CEN", 211.665, -36.36, 218.865, -42.14},
    {"CEN", 218.865, -42.14, 208.860, -47.28},
    {"CEN", 218.865, -42.14, 224.790, -42.09},
    {"CEP", 319.650,  62.58, 332.715,  58.20},
    {"CEP", 322.170,  70.56, 319.650,  62.58},
    {"CEP", 322.170,  70.56, 342.420,  66.20},
    {"CEP", 332.715,  58.20, 342.420,  66.20},
    {"CEP", 342.420,  66.20, 354.825,  77.63},
    {"CEP", 354.825,  77.63, 322.170,  70.56},
    {"CET",  17.145, -10.18,  20.985,  -8.18},
    {"CET",  20.985,  -8.18,  27.870, -10.33},
    {"CET",  26.010, -15.93,  30.000, -21.08},
    {"CET",  27.870, -10.33,  26.010, -15.93},
    {"CET",  27.870, -10.33,  34.815,  -2.98},
    {"CET",  27.870, -10.33,  41.010, -13.86},
    {"CET",  34.815,  -2.98,  39.870,   0.33},
    {"CET",  37.050,   8.46,  40.815,   3.23},
    {"CET",  39.870,   0.33,  40.815,   3.23},
    {"CET",  40.815,   3.23,  45.570,   4.08},
    {"CET",  41.220,  10.11,  37.050,   8.46},
    {"CET",  44.925,   8.89,  41.220,  10.11},
    {"CET",  45.570,   4.08,  44.925,   8.89},
    {"CET",   4.845,  -8.81,  10.890, -17.98},
    {"CET",  10.890, -17.98,  17.145, -10.18},
    {"CIR", 220.620, -64.98, 229.365, -58.79},
    {"CIR", 220.620, -64.98, 230.835, -59.31},
    {"CMA", 101.265, -16.71,  95.670, -17.95},
    {"CMA", 101.265, -16.71, 105.735, -23.83},
    {"CMA", 104.640, -28.96,  95.070, -30.06},
    {"CMA", 105.420, -27.93, 104.640, -28.96},
    {"CMA", 105.735, -23.83, 107.100, -26.39},
    {"CMA", 105.945, -15.63, 101.265, -16.71},
    {"CMA", 107.100, -26.39, 105.420, -27.93},
    {"CMA", 107.100, -26.39, 111.015, -29.30},
    {"CMI", 114.810,   5.21, 111.765,   8.28},
    {"CNC", 130.815,  21.46, 131.670,  28.76},
    {"CNC", 131.160,  18.14, 124.125,   9.18},
    {"CNC", 131.160,  18.14, 130.815,  21.46},
    {"CNC", 134.610,  11.85, 131.160,  18.14},
    {"COL",  82.800, -35.46,  84.900, -34.06},
    {"COL",  84.900, -34.06,  87.735, -35.76},
    {"COL",  87.735, -35.76,  89.370, -35.28},
    {"COL",  87.735, -35.76,  89.775, -42.81},
    {"COL",  89.370, -35.28,  95.520, -33.43},
    {"CRA", 164.940, -18.29, 162.390, -16.19},
    {"CRA", 164.940, -18.29, 170.520, -16.22},
    {"CRA", 167.925, -22.83, 164.940, -18.29},
    {"CRA", 169.815, -14.78, 171.135, -10.86},
    {"CRA", 171.135, -10.86, 174.165,  -9.80},
    {"CRA", 171.210, -17.68, 169.815, -14.78},
    {"CRA", 176.190, -18.34, 171.210, -17.68},
    {"CRA", 178.995, -17.14, 176.190, -18.34},
    {"CRA", 278.370, -42.31, 282.195, -43.68},
    {"CRA", 282.195, -43.68, 285.765, -42.09},
    {"CRA", 285.765, -42.09, 287.070, -40.50},
    {"CRA", 286.590, -37.06, 284.670, -37.10},
    {"CRA", 287.070, -40.50, 287.490, -39.33},
    {"CRA", 287.370, -37.89, 286.590, -37.06},
    {"CRA", 287.490, -39.33, 287.370, -37.89},
    {"CRU", 187.785, -57.11, 186.645, -63.09},
    {"CRU", 191.910, -59.68, 183.765, -58.75},
    {"CRV", 182.100, -24.73, 182.520, -22.61},
    {"CRV", 182.520, -22.61, 188.595, -23.40},
    {"CRV", 183.945, -17.55, 182.520, -22.61},
    {"CRV", 187.470, -16.51, 183.945, -17.55},
    {"CRV", 188.595, -23.40, 187.470, -16.51},
    {"CVN", 193.995,  38.31, 188.415,  41.35},
    {"CVN", 193.995,  38.31, 199.365,  40.56},
    {"CYG", 299.070,  35.08, 292.665,  27.96},
    {"CYG", 305.550,  40.25, 296.250,  45.13},
    {"CYG", 305.550,  40.25, 299.070,  35.08},
    {"CYG", 305.550,  40.25, 311.550,  33.96},
    {"CYG", 311.550,  33.96, 318.250,  30.23},
    {"CYG", 310.350,  45.28, 305.550,  40.25},
    {"DEL", 309.375,  14.60, 308.295,  11.29},
    {"DEL", 309.375,  14.60, 309.900,  15.91},
    {"DEL", 309.900,  15.91, 311.670,  16.11},
    {"DEL", 310.875,  15.06, 309.375,  14.60},
    {"DEL", 311.670,  16.11, 310.875,  15.06},
    {"DOR",  68.490, -55.04,  63.990, -51.48},
    {"DOR",  83.400, -62.48,  68.490, -55.04},
    {"DOR",  86.190, -65.73,  83.400, -62.48},
    {"DRA", 188.370,  69.78, 172.845,  69.33},
    {"DRA", 211.095,  64.38, 188.370,  69.78},
    {"DRA", 231.210,  58.96, 211.095,  64.38},
    {"DRA", 240.465,  58.56, 231.210,  58.96},
    {"DRA", 245.985,  61.51, 240.465,  58.56},
    {"DRA", 257.190,  65.71, 245.985,  61.51},
    {"DRA", 257.190,  65.71, 275.190,  71.33},
    {"DRA", 262.590,  52.30, 263.040,  55.18},
    {"DRA", 263.040,  55.18, 268.365,  56.86},
    {"DRA", 268.365,  56.86, 269.145,  51.48},
    {"DRA", 268.365,  56.86, 282.795,  59.38},
    {"DRA", 269.145,  51.48, 262.590,  52.30},
    {"DRA", 275.190,  71.33, 288.135,  67.66},
    {"DRA", 275.265,  72.73, 257.190,  65.71},
    {"DRA", 282.795,  59.38, 300.690,  67.86},
    {"DRA", 288.135,  67.66, 290.160,  65.71},
    {"DRA", 288.870,  73.34, 275.265,  72.73},
    {"DRA", 290.160,  65.71, 268.365,  56.86},
    {"DRA", 297.045,  70.26, 288.870,  73.34},
    {"DRA", 300.690,  67.86, 297.045,  70.26},
    {"EQU", 318.615,   9.99, 317.565,  10.13},
    {"EQU", 318.945,   5.25, 318.615,   9.99},
    {"ERI",  24.420, -57.23,  28.995, -51.61},
    {"ERI",  28.995, -51.61,  34.110, -51.51},
    {"ERI",  34.110, -51.51,  36.735, -47.69},
    {"ERI",  36.735, -47.69,  39.945, -42.88},
    {"ERI",  39.945, -42.88,  40.170, -39.85},
    {"ERI",  40.170, -39.85,  44.565, -40.29},
    {"ERI",  41.010, -13.86,  44.085,  -8.90},
    {"ERI",  41.265, -18.56,  41.010, -13.86},
    {"ERI",  41.265, -18.56,  45.600, -23.61},
    {"ERI",  44.085,  -8.90,  48.945,  -8.81},
    {"ERI",  44.565, -40.29,  55.695, -37.31},
    {"ERI",  45.600, -23.61,  49.860, -21.75},
    {"ERI",  48.945,  -8.81,  53.220,  -9.44},
    {"ERI",  49.860, -21.75,  53.445, -21.63},
    {"ERI",  53.220,  -9.44,  55.785,  -9.76},
    {"ERI",  53.445, -21.63,  56.685, -23.24},
    {"ERI",  55.785,  -9.76,  56.520, -12.10},
    {"ERI",  56.520, -12.10,  59.490, -13.51},
    {"ERI",  56.685, -23.24,  58.425, -24.61},
    {"ERI",  57.150, -37.61,  55.695, -37.31},
    {"ERI",  58.425, -24.61,  59.970, -24.01},
    {"ERI",  59.490, -13.51,  63.375,  -7.25},
    {"ERI",  59.970, -24.01,  68.370, -29.76},
    {"ERI",  63.375,  -7.25,  69.060,  -3.35},
    {"ERI",  64.470, -33.80,  57.150, -37.61},
    {"ERI",  66.000, -34.01,  64.470, -33.80},
    {"ERI",  68.370, -29.76,  68.895, -30.56},
    {"ERI",  68.895, -30.56,  66.000, -34.01},
    {"ERI",  69.060,  -3.35,  71.370,  -3.25},
    {"ERI",  71.370,  -3.25,  76.950,  -5.08},
    {"GEM",  93.720,  22.49,  91.020,  23.26},
    {"GEM",  95.745,  22.51,  93.720,  22.49},
    {"GEM",  99.420,  16.40, 106.020,  20.56},
    {"GEM", 100.965,  25.13,  95.745,  22.51},
    {"GEM", 101.325,  12.90,  99.420,  16.40},
    {"GEM", 106.020,  20.56, 110.010,  21.98},
    {"GEM", 110.010,  21.98, 116.325,  28.03},
    {"GEM", 113.640,  31.88, 100.965,  25.13},
    {"GEM", 116.325,  28.03, 113.640,  31.88},
    {"GRU", 328.470, -37.36, 331.515, -39.55},
    {"GRU", 331.515, -39.55, 333.900, -41.35},
    {"GRU", 333.900, -41.35, 337.320, -43.50},
    {"GRU", 337.320, -43.50, 332.040, -46.96},
    {"GRU", 337.320, -43.50, 340.665, -46.88},
    {"GRU", 340.665, -46.88, 342.150, -51.31},
    {"GRU", 340.665, -46.88, 347.595, -45.25},
    {"GRU", 342.150, -51.31, 345.225, -52.74},
    {"GRU", 347.595, -45.25, 346.710, -43.51},
    {"HER", 242.190,  44.93, 238.170,  42.44},
    {"HER", 244.920,  46.31, 242.190,  44.93},
    {"HER", 247.545,  21.48, 245.475,  19.14},
    {"HER", 248.520,  42.43, 244.920,  46.31},
    {"HER", 250.320,  31.59, 247.545,  21.48},
    {"HER", 250.320,  31.59, 250.710,  38.91},
    {"HER", 250.710,  38.91, 248.520,  42.43},
    {"HER", 250.710,  38.91, 258.750,  36.81},
    {"HER", 255.075,  30.93, 250.320,  31.59},
    {"HER", 258.645,  14.38, 258.750,  24.83},
    {"HER", 258.750,  24.83, 255.075,  30.93},
    {"HER", 258.750,  24.83, 262.665,  26.11},
    {"HER", 258.750,  36.81, 255.075,  30.93},
    {"HER", 258.750,  36.81, 260.910,  37.14},
    {"HER", 258.750,  36.81, 264.870,  46.00},
    {"HER", 260.910,  37.14, 269.070,  37.25},
    {"HER", 262.665,  26.11, 266.610,  27.71},
    {"HER", 266.610,  27.71, 269.445,  29.25},
    {"HER", 269.445,  29.25, 271.860,  28.76},
    {"HOR",  39.345, -52.54,  40.170, -54.55},
    {"HOR",  40.170, -54.55,  45.900, -59.73},
    {"HOR",  40.650, -50.79,  39.345, -52.54},
    {"HOR",  45.900, -59.73,  44.685, -64.06},
    {"HOR",  63.495, -42.30,  40.650, -50.79},
    {"HYA", 151.260, -13.06, 147.870, -14.84},
    {"HYA", 152.640, -12.35, 151.260, -13.06},
    {"HYA", 156.525, -16.83, 152.640, -12.35},
    {"HYA", 162.390, -16.19, 156.525, -16.83},
    {"HYA", 173.235, -31.84, 162.390, -16.19},
    {"HYA", 178.215, -33.89, 173.235, -31.84},
    {"HYA", 199.710, -23.16, 178.215, -33.89},
    {"HYA", 211.590, -26.68, 199.710, -23.16},
    {"HYA", 129.420,   5.70, 129.690,   3.33},
    {"HYA", 129.690,   3.33, 130.785,   3.39},
    {"HYA", 130.785,   3.39, 133.845,   5.95},
    {"HYA", 131.685,   6.41, 129.420,   5.70},
    {"HYA", 133.845,   5.95, 131.685,   6.41},
    {"HYA", 138.585,   2.31, 133.845,   5.95},
    {"HYA", 141.900,  -8.66, 144.960,  -1.15},
    {"HYA", 144.960,  -1.15, 138.585,   2.31},
    {"HYA", 147.870, -14.84, 141.900,  -8.66},
    {"HYI",  29.700, -61.56,   6.450, -77.24},
    {"HYI",  39.900, -68.26,  29.700, -61.56},
    {"HYI",  56.790, -74.23,  39.900, -68.26},
    {"HYI",   6.450, -77.24,  56.790, -74.23},
    {"IND", 309.390, -47.28, 319.965, -53.44},
    {"IND", 319.965, -53.44, 313.695, -58.44},
    {"IND", 319.965, -53.44, 329.475, -55.00},
    {"LAC", 335.235,  46.53, 340.110,  44.28},
    {"LAC", 335.895,  52.23, 337.815,  50.28},
    {"LAC", 336.120,  49.48, 337.365,  47.69},
    {"LAC", 337.365,  47.69, 335.235,  46.53},
    {"LAC", 337.815,  50.28, 336.120,  49.48},
    {"LAC", 340.110,  44.28, 333.990,  37.75},
    {"LEO", 151.815,  16.76, 154.995,  19.85},
    {"LEO", 152.100,  11.96, 151.815,  16.76},
    {"LEO", 154.170,  23.41, 148.200,  26.00},
    {"LEO", 154.995,  19.85, 154.170,  23.41},
    {"LEO", 168.525,  20.51, 168.540,  15.43},
    {"LEO", 168.540,  15.43, 152.100,  11.96},
    {"LEO", 168.540,  15.43, 177.270,  14.56},
    {"LEO", 177.270,  14.56, 168.525,  20.51},
    {"LEO", 148.200,  26.00, 146.460,  23.76},
    {"LEP",  82.035, -20.76,  76.365, -22.36},
    {"LEP",  82.035, -20.76,  86.115, -22.44},
    {"LEP",  83.175, -17.81,  78.210, -16.19},
    {"LEP",  83.175, -17.81,  82.035, -20.76},
    {"LEP",  86.115, -22.44,  87.825, -20.88},
    {"LEP",  86.745, -14.81,  83.175, -17.81},
    {"LEP",  87.825, -20.88,  83.175, -17.81},
    {"LEP",  89.085, -14.16,  86.745, -14.81},
    {"LIB", 222.720, -16.05, 229.245,  -9.38},
    {"LIB", 226.020, -25.28, 222.720, -16.05},
    {"LIB", 226.020, -25.28, 234.240, -28.13},
    {"LIB", 229.245,  -9.38, 233.865, -14.78},
    {"LIB", 233.865, -14.78, 226.020, -25.28},
    {"LIB", 234.240, -28.13, 234.660, -29.78},
    {"LMI", 156.960,  36.70, 151.845,  35.25},
    {"LMI", 156.960,  36.70, 163.320,  34.21},
    {"LUP", 220.470, -47.38, 228.075, -52.10},
    {"LUP", 224.610, -43.13, 220.470, -47.38},
    {"LUP", 228.075, -52.10, 230.670, -44.68},
    {"LUP", 230.340, -40.64, 224.610, -43.13},
    {"LUP", 230.445, -36.26, 230.340, -40.64},
    {"LUP", 230.670, -44.68, 233.760, -41.16},
    {"LUP", 235.290, -44.66, 233.760, -41.16},
    {"LUP", 237.750, -33.63, 230.445, -36.26},
    {"LUP", 241.650, -36.80, 235.290, -44.66},
    {"LUP", 241.650, -36.80, 237.750, -33.63},
    {"LYN",  94.890,  59.01, 104.325,  58.41},
    {"LYN", 104.325,  58.41, 111.660,  49.21},
    {"LYN", 111.660,  49.21, 122.115,  51.50},
    {"LYN", 122.115,  51.50, 125.700,  43.18},
    {"LYN", 139.695,  36.80, 125.700,  43.18},
    {"LYN", 140.265,  34.39, 139.695,  36.80},
    {"LYR", 279.225,  38.78, 281.070,  39.66},
    {"LYR", 279.225,  38.78, 281.190,  37.59},
    {"LYR", 281.190,  37.59, 282.510,  33.36},
    {"LYR", 282.510,  33.36, 284.715,  32.68},
    {"LYR", 283.620,  36.90, 281.190,  37.59},
    {"LYR", 284.715,  32.68, 283.620,  36.90},
    {"MEN",  73.785, -74.93,  75.675, -71.31},
    {"MEN",  82.965, -76.33,  73.785, -74.93},
    {"MEN",  92.550, -74.74,  82.965, -76.33},
    {"MIC", 312.495, -33.78, 315.315, -32.25},
    {"MIC", 315.315, -32.25, 319.470, -32.16},
    {"MIC", 319.470, -32.16, 320.190, -40.81},
    {"MON",  97.200,  -7.03,  93.720,  -6.26},
    {"MON", 107.970,  -0.50,  97.200,  -7.03},
    {"MON", 115.290,  -9.55, 107.970,  -0.50},
    {"MON", 122.145,  -2.98, 115.290,  -9.55},
    {"MUS", 176.385, -66.73, 184.395, -67.96},
    {"MUS", 184.395, -67.96, 189.300, -69.13},
    {"MUS", 188.115, -72.13, 195.570, -71.55},
    {"MUS", 189.300, -69.13, 188.115, -72.13},
    {"MUS", 191.565, -68.10, 189.300, -69.13},
    {"MUS", 195.570, -71.55, 191.565, -68.10},
    {"NOR", 240.795, -49.23, 244.950, -50.15},
    {"NOR", 241.620, -45.16, 240.795, -49.23},
    {"NOR", 244.950, -50.15, 246.795, -47.55},
    {"NOR", 246.795, -47.55, 241.620, -45.16},
    {"OCT", 216.720, -83.66, 325.365, -77.38},
    {"OCT", 325.365, -77.38, 341.520, -81.38},
    {"OCT", 341.520, -81.38, 216.720, -83.66},
    {"OPH", 243.570,  -3.70, 244.560,  -4.69},
    {"OPH", 244.560,  -4.69, 249.285, -10.56},
    {"OPH", 247.710,   1.98, 243.570,  -3.70},
    {"OPH", 249.285, -10.56, 257.595, -15.71},
    {"OPH", 254.415,   9.38, 247.710,   1.98},
    {"OPH", 257.595, -15.71, 266.970,   2.70},
    {"OPH", 263.715,  12.56, 254.415,   9.38},
    {"OPH", 265.860,   4.56, 263.715,  12.56},
    {"OPH", 266.970,   2.70, 265.860,   4.56},
    {"OPH", 266.970,   2.70, 270.135,   2.93},
    {"OPH", 270.135,   2.93, 271.365,   2.50},
    {"ORI",  72.450,   6.96,  72.645,   8.89},
    {"ORI",  72.645,   8.89,  73.125,  14.25},
    {"ORI",  72.795,   5.60,  72.450,   6.96},
    {"ORI",  73.560,   2.43,  72.795,   5.60},
    {"ORI",  74.625,   1.71,  73.560,   2.43},
    {"ORI",  78.615,  -8.19,  86.940,  -9.66},
    {"ORI",  79.395,  -6.85,  78.615,  -8.19},
    {"ORI",  81.270,   6.35,  74.625,   1.71},
    {"ORI",  81.270,   6.35,  82.995,  -0.29},
    {"ORI",  82.995,  -0.29,  79.395,  -6.85},
    {"ORI",  83.685,   9.48,  81.270,   6.35},
    {"ORI",  85.200,  -1.95,  88.785,   7.40},
    {"ORI",  86.940,  -9.66,  85.200,  -1.95},
    {"ORI",  88.785,   7.40,  83.685,   9.48},
    {"ORI",  88.785,   7.40,  90.600,   9.65},
    {"ORI",  90.600,   9.65,  91.890,  14.76},
    {"ORI",  90.600,   9.65,  92.970,  14.21},
    {"ORI",  91.890,  14.76,  88.590,  20.28},
    {"ORI",  92.970,  14.21,  90.960,  20.13},
    {"PAV", 266.415, -64.71, 280.740, -71.43},
    {"PAV", 280.740, -71.43, 300.150, -72.91},
    {"PAV", 300.150, -72.91, 311.250, -66.20},
    {"PAV", 302.160, -66.18, 266.415, -64.71},
    {"PAV", 306.390, -56.73, 311.250, -66.20},
    {"PAV", 311.250, -66.20, 302.160, -66.18},
    {"PAV", 311.250, -66.20, 321.585, -65.36},
    {"PEG",   2.100,  29.08,   3.300,  15.18},
    {"PEG",   3.300,  15.18, 346.185,  15.20},
    {"PEG", 331.740,  25.34, 326.145,  25.65},
    {"PEG", 332.550,   6.20, 326.040,   9.88},
    {"PEG", 340.365,  10.83, 332.550,   6.20},
    {"PEG", 340.740,  30.21, 332.490,  33.18},
    {"PEG", 341.610,  23.56, 331.740,  25.34},
    {"PEG", 341.670,  12.16, 340.365,  10.83},
    {"PEG", 342.495,  24.60, 341.610,  23.56},
    {"PEG", 345.945,  28.08,   2.100,  29.08},
    {"PEG", 345.945,  28.08, 340.740,  30.21},
    {"PEG", 345.945,  28.08, 342.495,  24.60},
    {"PEG", 346.185,  15.20, 341.670,  12.16},
    {"PEG", 346.185,  15.20, 345.945,  28.08},
    {"PER",  42.660,  55.89,  43.560,  52.76},
    {"PER",  42.660,  55.89,  46.200,  53.50},
    {"PER",  43.560,  52.76,  47.265,  49.61},
    {"PER",  46.200,  53.50,  51.060,  49.86},
    {"PER",  47.040,  40.94,  46.290,  38.83},
    {"PER",  47.265,  49.61,  47.370,  44.84},
    {"PER",  47.370,  44.84,  47.040,  40.94},
    {"PER",  51.060,  49.86,  55.710,  47.78},
    {"PER",  55.710,  47.78,  56.295,  42.58},
    {"PER",  55.710,  47.78,  62.175,  47.71},
    {"PER",  56.295,  42.58,  59.475,  40.01},
    {"PER",  58.515,  31.88,  56.070,  32.28},
    {"PER",  59.475,  40.01,  59.745,  35.78},
    {"PER",  59.745,  35.78,  58.515,  31.88},
    {"PER",  62.175,  47.71,  63.720,  48.41},
    {"PER",  63.720,  48.41,  61.650,  50.35},
    {"PHE",  16.515, -46.71,  22.815, -49.06},
    {"PHE",  16.515, -46.71,   6.570, -42.30},
    {"PHE",  17.085, -55.25,  16.515, -46.71},
    {"PHE",  22.095, -43.31,  16.515, -46.71},
    {"PHE",  22.815, -49.06,  22.095, -43.31},
    {"PHE",   6.570, -42.30,   2.340, -45.74},
    {"PIC",  87.450, -56.16,  86.820, -51.06},
    {"PIC", 102.045, -61.93,  87.450, -56.16},
    {"PSA", 326.220, -33.03, 332.085, -32.98},
    {"PSA", 332.085, -32.98, 337.860, -32.34},
    {"PSA", 337.860, -32.34, 343.125, -32.88},
    {"PSA", 343.125, -32.88, 343.965, -32.53},
    {"PSA", 343.965, -32.53, 344.400, -29.61},
    {"PSA", 344.400, -29.61, 340.170, -27.05},
    {"PSA", 344.400, -29.61, 347.340, -21.16},
    {"PSC",  15.720,   7.88,  12.165,   7.58},
    {"PSC",  18.420,  24.58,  19.875,  27.26},
    {"PSC",  19.875,  27.26,  17.910,  30.08},
    {"PSC",  22.875,  15.35,  18.420,  24.58},
    {"PSC",  25.350,   5.48,  15.720,   7.88},
    {"PSC",  26.340,   9.15,  22.875,  15.35},
    {"PSC",  30.495,   2.76,  25.350,   5.48},
    {"PSC",  30.495,   2.76,  26.340,   9.15},
    {"PSC", 349.290,   3.28, 345.960,   3.81},
    {"PSC", 349.290,   3.28, 351.990,   6.38},
    {"PSC", 351.720,   1.25, 349.290,   3.28},
    {"PSC", 351.990,   6.38, 354.975,   5.63},
    {"PSC", 354.975,   5.63, 355.500,   1.78},
    {"PSC", 355.500,   1.78, 351.720,   1.25},
    {"PSC", 359.820,   6.86, 354.975,   5.63},
    {"PSC",  12.165,   7.58, 359.820,   6.86},
    {"PUP",  99.435, -43.20, 102.465, -50.61},
    {"PUP", 102.465, -50.61, 108.840, -43.98},
    {"PUP", 108.840, -43.98,  99.435, -43.20},
    {"PUP", 108.840, -43.98, 112.290, -43.29},
    {"PUP", 109.260, -37.09, 120.885, -40.00},
    {"PUP", 112.290, -43.29, 109.260, -37.09},
    {"PUP", 115.935, -28.95, 120.885, -40.00},
    {"PUP", 117.315, -24.86, 115.935, -28.95},
    {"PUP", 120.885, -40.00, 112.290, -43.29},
    {"PUP", 121.875, -24.29, 117.315, -24.86},
    {"PYX", 130.020, -35.30, 130.890, -33.18},
    {"PYX", 130.890, -33.18, 132.615, -27.71},
    {"RET",  56.040, -64.80,  63.600, -62.46},
    {"RET",  59.670, -61.39,  60.210, -62.16},
    {"RET",  60.210, -62.16,  56.040, -64.80},
    {"RET",  63.600, -62.46,  64.125, -59.29},
    {"RET",  64.125, -59.29,  59.670, -61.39},
    {"SCL", 349.695, -32.53, 357.225, -28.13},
    {"SCL", 353.250, -37.81, 349.695, -32.53},
    {"SCL", 357.225, -28.13,  14.640, -29.35},
    {"SCO", 240.060, -22.61, 239.685, -26.11},
    {"SCO", 240.060, -22.61, 241.350, -19.80},
    {"SCO", 240.060, -22.61, 245.295, -25.60},
    {"SCO", 245.295, -25.60, 247.350, -26.43},
    {"SCO", 247.350, -26.43, 248.970, -28.21},
    {"SCO", 248.970, -28.21, 252.540, -34.30},
    {"SCO", 252.540, -34.30, 252.960, -38.05},
    {"SCO", 252.960, -38.05, 253.635, -42.36},
    {"SCO", 253.635, -42.36, 258.045, -43.23},
    {"SCO", 258.045, -43.23, 264.315, -42.99},
    {"SCO", 263.400, -37.10, 262.695, -37.30},
    {"SCO", 264.315, -42.99, 266.895, -40.13},
    {"SCO", 265.620, -39.03, 263.400, -37.10},
    {"SCO", 266.895, -40.13, 265.620, -39.03},
    {"SCT", 277.290, -14.56, 278.790,  -8.25},
    {"SCT", 278.790,  -8.25, 281.790,  -4.74},
    {"SER", 233.685,  10.53, 236.070,   6.43},
    {"SER", 236.070,   6.43, 237.690,   4.48},
    {"SER", 236.550,  15.41, 233.685,  10.53},
    {"SER", 237.165,  18.13, 235.395,  19.66},
    {"SER", 237.165,  18.13, 236.550,  15.41},
    {"SER", 237.165,  18.13, 237.825,  20.98},
    {"SER", 237.165,  18.13, 239.115,  15.66},
    {"SER", 237.390,  -3.43, 243.570,  -3.70},
    {"SER", 237.690,   4.48, 237.390,  -3.43},
    {"SER", 260.190, -12.84, 257.595, -15.71},
    {"SER", 264.390, -15.40, 260.190, -12.84},
    {"SER", 265.350, -12.88, 264.390, -15.40},
    {"SER", 269.745,  -9.76, 265.350, -12.88},
    {"SER", 275.310,  -2.90, 269.745,  -9.76},
    {"SER", 284.040,   4.19, 275.310,  -2.90},
    {"SEX", 151.965,  -0.36, 157.560,  -0.63},
    {"SEX", 157.365,  -2.73, 148.110,  -8.09},
    {"SEX", 157.560,  -0.63, 157.365,  -2.73},
    {"SEX", 148.110,  -8.09, 151.965,  -0.36},
    {"SGE", 296.835,  18.53, 295.020,  18.01},
    {"SGE", 296.835,  18.53, 295.245,  17.48},
    {"SGE", 299.700,  19.49, 296.835,  18.53},
    {"SGR", 271.440, -30.41, 276.045, -34.38},
    {"SGR", 275.250, -29.83, 271.440, -30.41},
    {"SGR", 276.045, -34.38, 275.250, -29.83},
    {"SGR", 276.045, -34.38, 285.645, -29.88},
    {"SGR", 276.990, -25.41, 275.250, -29.83},
    {"SGR", 281.415, -26.98, 276.990, -25.41},
    {"SGR", 283.815, -26.29, 281.415, -26.98},
    {"SGR", 283.815, -26.29, 286.170, -21.75},
    {"SGR", 285.645, -29.88, 281.415, -26.98},
    {"SGR", 285.645, -29.88, 286.725, -27.66},
    {"SGR", 286.170, -21.75, 284.415, -21.10},
    {"SGR", 286.170, -21.75, 287.445, -21.01},
    {"SGR", 286.725, -27.66, 283.815, -26.29},
    {"SGR", 287.445, -21.01, 290.415, -17.85},
    {"SGR", 290.640, -44.46, 290.970, -40.61},
    {"SGR", 290.970, -40.61, 285.645, -29.88},
    {"TAU",  51.795,   9.73,  51.195,   9.03},
    {"TAU",  60.165,  12.48,  51.795,   9.73},
    {"TAU",  64.935,  15.63,  60.165,  12.48},
    {"TAU",  65.715,  17.55,  64.935,  15.63},
    {"TAU",  66.375,  17.93,  65.715,  17.55},
    {"TAU",  67.140,  15.96,  64.935,  15.63},
    {"TAU",  67.140,  19.18,  66.375,  17.93},
    {"TAU",  68.970,  16.51,  67.140,  15.96},
    {"TAU",  70.545,  22.95,  67.140,  19.18},
    {"TAU",  81.570,  28.60,  70.545,  22.95},
    {"TAU",  84.390,  21.15,  68.970,  16.51},
    {"TEL", 276.750, -45.96, 272.790, -45.94},
    {"TEL", 277.200, -49.06, 276.750, -45.96},
    {"TRA", 229.710, -68.68, 252.165, -69.03},
    {"TRA", 234.165, -66.31, 229.710, -68.68},
    {"TRA", 238.770, -63.43, 234.165, -66.31},
    {"TRA", 252.165, -69.03, 238.770, -63.43},
    {"TRI",  28.275,  29.58,  32.370,  34.98},
    {"TRI",  28.275,  29.58,  34.320,  33.85},
    {"TRI",  32.370,  34.98,  34.320,  33.85},
    {"TUC", 334.620, -60.26, 336.825, -64.96},
    {"TUC", 349.335, -58.23, 334.620, -60.26},
    {"TUC", 349.335, -58.23,   7.860, -62.95},
    {"TUC", 359.970, -65.58, 336.825, -64.96},
    {"TUC",   5.010, -64.88, 359.970, -65.58},
    {"TUC",   7.860, -62.95,   5.010, -64.88},
    {"UMA", 165.450,  56.38, 165.915,  61.74},
    {"UMA", 165.915,  61.74, 183.840,  57.03},
    {"UMA", 165.915,  61.74, 142.875,  63.06},
    {"UMA", 167.415,  44.50, 155.565,  41.49},
    {"UMA", 169.620,  33.09, 169.545,  31.53},
    {"UMA", 176.490,  47.78, 167.415,  44.50},
    {"UMA", 176.490,  47.78, 169.620,  33.09},
    {"UMA", 178.440,  53.69, 165.450,  56.38},
    {"UMA", 178.440,  53.69, 176.490,  47.78},
    {"UMA", 183.840,  57.03, 178.440,  53.69},
    {"UMA", 193.500,  55.96, 183.840,  57.03},
    {"UMA", 200.970,  54.93, 193.500,  55.96},
    {"UMA", 206.865,  49.31, 200.970,  54.93},
    {"UMA", 134.790,  48.05, 143.220,  51.68},
    {"UMA", 142.875,  63.06, 127.575,  60.71},
    {"UMA", 143.220,  51.68, 148.020,  54.06},
    {"UMA", 147.750,  59.03, 142.875,  63.06},
    {"UMA", 148.020,  54.06, 147.750,  59.03},
    {"UMI", 222.660,  74.14, 230.160,  71.83},
    {"UMI", 230.160,  71.83, 244.365,  75.75},
    {"UMI", 236.025,  77.79, 222.660,  74.14},
    {"UMI", 244.365,  75.75, 236.025,  77.79},
    {"UMI", 251.490,  82.03, 236.025,  77.79},
    {"UMI", 263.040,  86.58, 251.490,  82.03},
    {"UMI",  37.950,  89.26, 263.040,  86.58},
    {"VEL", 161.685, -49.41, 149.220, -54.56},
    {"VEL", 122.370, -47.34, 136.995, -43.43},
    {"VEL", 131.160, -54.69, 122.370, -47.34},
    {"VEL", 136.995, -43.43, 140.520, -55.01},
    {"VEL", 136.995, -43.43, 142.665, -40.46},
    {"VEL", 140.520, -55.01, 131.160, -54.69},
    {"VEL", 149.220, -54.56, 140.520, -55.01},
    {"VIR", 184.965,  -0.66, 177.675,   1.76},
    {"VIR", 190.410,  -1.45, 184.965,  -0.66},
    {"VIR", 190.410,  -1.45, 201.300, -11.16},
    {"VIR", 193.890,   3.40, 190.410,  -1.45},
    {"VIR", 195.540,  10.96, 193.890,   3.40},
    {"VIR", 197.490,  17.53, 197.970,  27.88},
    {"VIR", 197.970,  27.88, 186.720,  28.26},
    {"VIR", 201.300, -11.16, 213.990,  -5.99},
    {"VIR", 210.390,   1.54, 203.670,  -0.60},
    {"VIR", 213.990,  -5.99, 220.770,  -5.66},
    {"VIR", 220.770,  -5.66, 221.535,   1.90},
    {"VIR", 221.535,   1.90, 210.390,   1.54},
    {"VIR", 231.945,  29.10, 233.670,  26.71},
    {"VIR", 233.220,  31.36, 231.945,  29.10},
    {"VIR", 233.670,  26.71, 235.665,  26.30},
    {"VIR", 235.665,  26.30, 237.390,  26.06},
    {"VIR", 237.390,  26.06, 239.400,  26.88},
    {"VIR", 239.400,  26.88, 240.345,  29.85},
    {"VOL", 107.190, -70.50, 109.185, -67.94},
    {"VOL", 109.185, -67.94, 126.420, -66.13},
    {"VOL", 115.440, -72.59, 107.190, -70.50},
    {"VOL", 121.965, -68.61, 115.440, -72.59},
    {"VOL", 126.420, -66.13, 121.965, -68.61},
    {"VOL", 135.585, -66.39, 126.420, -66.13},
    {"VOL", 137.745, -58.96, 139.275, -59.28},
    {"VUL", 292.170,  24.66, 289.035,  21.38},
    {"VUL", 292.170,  24.66, 298.365,  24.08},
    {"",      0.000,   0.00,   0.000,   0.00}
};

