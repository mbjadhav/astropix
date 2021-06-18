EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 1 1
Title "AstroPix2 Carrier"
Date "2021-06-17"
Rev "1.0"
Comp "Nicolas Striebig, KIT IPE"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:VDD #PWR0101
U 1 1 60D6EF8A
P 12800 5050
F 0 "#PWR0101" H 12800 4900 50  0001 C CNN
F 1 "VDD" H 12815 5223 50  0000 C CNN
F 2 "" H 12800 5050 50  0001 C CNN
F 3 "" H 12800 5050 50  0001 C CNN
	1    12800 5050
	-1   0    0    -1  
$EndComp
$Comp
L power:VDDA #PWR0102
U 1 1 60D6FB3F
P 12650 700
F 0 "#PWR0102" H 12650 550 50  0001 C CNN
F 1 "VDDA" H 12665 873 50  0000 C CNN
F 2 "" H 12650 700 50  0001 C CNN
F 3 "" H 12650 700 50  0001 C CNN
	1    12650 700 
	-1   0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR0103
U 1 1 60D703AC
P 12350 4600
F 0 "#PWR0103" H 12350 4350 50  0001 C CNN
F 1 "GNDA" H 12355 4427 50  0000 C CNN
F 2 "" H 12350 4600 50  0001 C CNN
F 3 "" H 12350 4600 50  0001 C CNN
	1    12350 4600
	-1   0    0    -1  
$EndComp
Wire Wire Line
	12350 4600 12350 4550
Wire Wire Line
	12650 4550 12650 4400
Connection ~ 12550 4550
Wire Wire Line
	12550 4550 12650 4550
Connection ~ 12450 4550
Wire Wire Line
	12450 4550 12550 4550
Connection ~ 12350 4550
Wire Wire Line
	12350 4550 12450 4550
Wire Wire Line
	12650 700  12650 750 
Wire Wire Line
	12950 750  12950 900 
Wire Wire Line
	12850 750  12850 900 
Connection ~ 12850 750 
Wire Wire Line
	12850 750  12950 750 
Connection ~ 12750 750 
Wire Wire Line
	12750 750  12850 750 
Connection ~ 12650 750 
Wire Wire Line
	12650 750  12750 750 
$Comp
L astropix:suba #PWR0104
U 1 1 60D7C76A
P 12950 4600
F 0 "#PWR0104" H 12900 4400 50  0001 C CNN
F 1 "suba" H 12955 4427 50  0000 C CNN
F 2 "" H 12950 4600 50  0001 C CNN
F 3 "" H 12950 4600 50  0001 C CNN
	1    12950 4600
	-1   0    0    -1  
$EndComp
Wire Wire Line
	12950 4600 12950 4550
Wire Wire Line
	13600 4550 13600 4400
Connection ~ 13500 4550
Wire Wire Line
	13500 4550 13600 4550
Connection ~ 13400 4550
Wire Wire Line
	13400 4550 13500 4550
Connection ~ 13300 4550
Wire Wire Line
	13300 4550 13400 4550
Connection ~ 13200 4550
Wire Wire Line
	13200 4550 13300 4550
Connection ~ 13100 4550
Wire Wire Line
	13100 4550 13200 4550
Wire Wire Line
	12800 5050 12800 5100
Wire Wire Line
	12650 9600 12550 9600
Connection ~ 12550 9600
Wire Wire Line
	12900 5100 12800 5100
Connection ~ 12800 5100
$Comp
L astropix:subd #PWR0105
U 1 1 60D8706B
P 13050 9650
F 0 "#PWR0105" H 13000 9450 50  0001 C CNN
F 1 "subd" H 13055 9477 50  0000 C CNN
F 2 "" H 13050 9650 50  0001 C CNN
F 3 "" H 13050 9650 50  0001 C CNN
	1    13050 9650
	-1   0    0    -1  
$EndComp
Wire Wire Line
	13050 9650 13050 9600
Wire Wire Line
	13050 9600 13050 9450
Wire Wire Line
	13050 9600 13150 9600
Wire Wire Line
	13150 9600 13150 9450
Connection ~ 13050 9600
$Comp
L Amplifier_Operational:OPA340NA U1
U 1 1 60D8C8A5
P 8800 1500
F 0 "U1" H 8450 1800 50  0000 L CNN
F 1 "OPA320" H 8850 1350 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 8700 1300 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/opa340.pdf" H 8800 1700 50  0001 C CNN
	1    8800 1500
	1    0    0    -1  
$EndComp
$Comp
L power:VDDA #PWR0106
U 1 1 60D8F9D6
P 2750 950
F 0 "#PWR0106" H 2750 800 50  0001 C CNN
F 1 "VDDA" H 2765 1123 50  0000 C CNN
F 2 "" H 2750 950 50  0001 C CNN
F 3 "" H 2750 950 50  0001 C CNN
	1    2750 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	8700 1150 8700 1200
$Comp
L power:GNDA #PWR0107
U 1 1 60D913F1
P 8700 1950
F 0 "#PWR0107" H 8700 1700 50  0001 C CNN
F 1 "GNDA" H 8705 1777 50  0000 C CNN
F 2 "" H 8700 1950 50  0001 C CNN
F 3 "" H 8700 1950 50  0001 C CNN
	1    8700 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8700 1800 8700 1950
Wire Wire Line
	8500 1600 8450 1600
Wire Wire Line
	8450 1600 8450 1850
Wire Wire Line
	9150 1850 9150 1500
Wire Wire Line
	9150 1500 9100 1500
Wire Wire Line
	8450 1850 9150 1850
$Comp
L Device:R R2
U 1 1 60D9635A
P 9450 1500
F 0 "R2" V 9243 1500 50  0000 C CNN
F 1 "22" V 9334 1500 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 9380 1500 50  0001 C CNN
F 3 "~" H 9450 1500 50  0001 C CNN
	1    9450 1500
	0    1    1    0   
$EndComp
Wire Wire Line
	9150 1500 9300 1500
Connection ~ 9150 1500
$Comp
L Connector:Conn_Coaxial J3
U 1 1 60D98ECC
P 9900 1500
F 0 "J3" H 10000 1475 50  0000 L CNN
F 1 "Vtemp1" H 10000 1384 50  0000 L CNN
F 2 "Connector_Coaxial:SMA_Molex_73251-2200_Horizontal" H 9900 1500 50  0001 C CNN
F 3 " ~" H 9900 1500 50  0001 C CNN
	1    9900 1500
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR0108
U 1 1 60D99A5E
P 2750 1600
F 0 "#PWR0108" H 2750 1350 50  0001 C CNN
F 1 "GNDA" H 2755 1427 50  0000 C CNN
F 2 "" H 2750 1600 50  0001 C CNN
F 3 "" H 2750 1600 50  0001 C CNN
	1    2750 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 1700 9900 1950
NoConn ~ 1100 3050
NoConn ~ 1100 3150
NoConn ~ 1100 2850
NoConn ~ 1100 2950
Wire Wire Line
	11650 7400 11350 7400
Wire Wire Line
	11650 7500 11350 7500
Wire Wire Line
	11650 7600 11350 7600
Wire Wire Line
	11650 7700 11350 7700
Text Label 11350 7400 0    50   ~ 0
sr_ck1
Text Label 11350 7500 0    50   ~ 0
sr_ck2
Text Label 11350 7600 0    50   ~ 0
sr_ld
Text Label 11350 7700 0    50   ~ 0
sr_sin
Text Label 11350 7800 0    50   ~ 0
sr_rb
Text Label 14350 7450 2    50   ~ 0
sr_sout
Wire Wire Line
	1100 5050 800  5050
Wire Wire Line
	1100 5150 800  5150
Wire Wire Line
	2850 5050 3150 5050
Wire Wire Line
	2850 5150 3150 5150
Text Label 800  5050 0    50   ~ 0
sr_ck1
Text Label 800  5150 0    50   ~ 0
sr_ck2
Text Label 3150 5050 2    50   ~ 0
sr_ld
Text Label 3150 5150 2    50   ~ 0
sr_sin
$Comp
L astropix:SN65LVDS4 U2
U 1 1 60DF4BC4
P 8900 8150
F 0 "U2" H 8450 8550 50  0000 L CNN
F 1 "SN65LVDS4" H 9250 7750 50  0000 L CNN
F 2 "astropix:Texas_Instruments-SN65LVDS4RSET-0" H 8900 8150 50  0001 C CNN
F 3 "" H 8900 8150 50  0001 C CNN
	1    8900 8150
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR0110
U 1 1 60DF5BD6
P 8850 8650
F 0 "#PWR0110" H 8850 8400 50  0001 C CNN
F 1 "GNDD" H 8854 8495 50  0000 C CNN
F 2 "" H 8850 8650 50  0001 C CNN
F 3 "" H 8850 8650 50  0001 C CNN
	1    8850 8650
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8850 8550 8850 8600
Wire Wire Line
	8850 8600 8950 8600
Wire Wire Line
	8950 8600 8950 8550
Connection ~ 8850 8600
Wire Wire Line
	8850 8600 8850 8650
$Comp
L power:VDD #PWR0111
U 1 1 60DFCC85
P 8850 7650
F 0 "#PWR0111" H 8850 7500 50  0001 C CNN
F 1 "VDD" H 8865 7823 50  0000 C CNN
F 2 "" H 8850 7650 50  0001 C CNN
F 3 "" H 8850 7650 50  0001 C CNN
	1    8850 7650
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8850 7650 8850 7700
Wire Wire Line
	8850 7700 8950 7700
Wire Wire Line
	8950 7700 8950 7750
Connection ~ 8850 7700
Wire Wire Line
	8850 7700 8850 7750
$Comp
L Device:R R4
U 1 1 60E01585
P 11100 6700
F 0 "R4" H 11170 6746 50  0000 L CNN
F 1 "100" H 11170 6655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 11030 6700 50  0001 C CNN
F 3 "~" H 11100 6700 50  0001 C CNN
	1    11100 6700
	1    0    0    -1  
$EndComp
Wire Wire Line
	11350 6650 11350 6500
Wire Wire Line
	11350 6500 11100 6500
Wire Wire Line
	11350 6650 11650 6650
Wire Wire Line
	11100 6900 11350 6900
Wire Wire Line
	11350 6900 11350 6750
Wire Wire Line
	11350 6750 11650 6750
Wire Wire Line
	11100 6850 11100 6900
Wire Wire Line
	11100 6550 11100 6500
Connection ~ 11100 6900
Text Label 10450 6900 0    50   ~ 0
sample_clock_N
Wire Wire Line
	10450 6900 11100 6900
Text Label 10950 7150 0    50   ~ 0
timestamp_clk
$Comp
L Device:R R5
U 1 1 60E2955B
P 10800 5900
F 0 "R5" H 10870 5946 50  0000 L CNN
F 1 "10k" H 10870 5855 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 10730 5900 50  0001 C CNN
F 3 "~" H 10800 5900 50  0001 C CNN
	1    10800 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	10800 6050 10800 6100
$Comp
L power:VDD #PWR0112
U 1 1 60E2E368
P 10800 5700
F 0 "#PWR0112" H 10800 5550 50  0001 C CNN
F 1 "VDD" H 10815 5873 50  0000 C CNN
F 2 "" H 10800 5700 50  0001 C CNN
F 3 "" H 10800 5700 50  0001 C CNN
	1    10800 5700
	-1   0    0    -1  
$EndComp
Wire Wire Line
	10800 5750 10800 5700
Wire Wire Line
	10950 7150 11650 7150
Text Label 10450 6100 0    50   ~ 0
res_n
$Comp
L Connector_Generic:Conn_01x03 J2
U 1 1 60E4C6FC
P 5650 3150
F 0 "J2" H 5730 3192 50  0000 L CNN
F 1 "VDD_JP" H 5730 3101 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 5650 3150 50  0001 C CNN
F 3 "~" H 5650 3150 50  0001 C CNN
	1    5650 3150
	1    0    0    -1  
$EndComp
$Comp
L power:VDD #PWR0113
U 1 1 60E54304
P 8250 7100
F 0 "#PWR0113" H 8250 6950 50  0001 C CNN
F 1 "VDD" H 8265 7273 50  0000 C CNN
F 2 "" H 8250 7100 50  0001 C CNN
F 3 "" H 8250 7100 50  0001 C CNN
	1    8250 7100
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5450 3150 5400 3150
Wire Wire Line
	5400 3150 5400 2850
Wire Wire Line
	2850 2950 2950 2950
Wire Wire Line
	2850 2850 2950 2850
Wire Wire Line
	5000 3050 5450 3050
Wire Wire Line
	2850 3050 2950 3050
Wire Wire Line
	2950 3050 2950 3150
Wire Wire Line
	2950 3150 2850 3150
Wire Wire Line
	5450 3250 5000 3250
Wire Wire Line
	2850 3250 2950 3250
Wire Wire Line
	2850 3350 2950 3350
Wire Wire Line
	2950 3350 2950 3250
Connection ~ 2950 3250
Wire Wire Line
	2950 3250 3450 3250
$Comp
L power:GND #PWR0114
U 1 1 60E8D615
P 3050 3650
F 0 "#PWR0114" H 3050 3400 50  0001 C CNN
F 1 "GND" H 3055 3477 50  0000 C CNN
F 2 "" H 3050 3650 50  0001 C CNN
F 3 "" H 3050 3650 50  0001 C CNN
	1    3050 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 3650 3050 3650
$Comp
L power:GND #PWR0115
U 1 1 60E91C7C
P 900 3650
F 0 "#PWR0115" H 900 3400 50  0001 C CNN
F 1 "GND" H 905 3477 50  0000 C CNN
F 2 "" H 900 3650 50  0001 C CNN
F 3 "" H 900 3650 50  0001 C CNN
	1    900  3650
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1100 3650 900  3650
NoConn ~ 2850 3450
NoConn ~ 2850 3550
NoConn ~ 1100 3550
NoConn ~ 1100 3450
NoConn ~ 1100 3350
NoConn ~ 1100 3250
$Comp
L power:VDDA #PWR0116
U 1 1 60EB1D2F
P 3450 2500
F 0 "#PWR0116" H 3450 2350 50  0001 C CNN
F 1 "VDDA" H 3465 2673 50  0000 C CNN
F 2 "" H 3450 2500 50  0001 C CNN
F 3 "" H 3450 2500 50  0001 C CNN
	1    3450 2500
	1    0    0    -1  
$EndComp
$Comp
L astropix:-HV #PWR0117
U 1 1 60EB6184
P 850 2650
F 0 "#PWR0117" H 800 2450 50  0001 C CNN
F 1 "-HV" H 855 2477 50  0000 C CNN
F 2 "" H 850 2650 50  0001 C CNN
F 3 "" H 850 2650 50  0001 C CNN
	1    850  2650
	1    0    0    -1  
$EndComp
$Comp
L astropix:-HV #PWR0118
U 1 1 60EB6F0B
P 3050 2650
F 0 "#PWR0118" H 3000 2450 50  0001 C CNN
F 1 "-HV" H 3055 2477 50  0000 C CNN
F 2 "" H 3050 2650 50  0001 C CNN
F 3 "" H 3050 2650 50  0001 C CNN
	1    3050 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 2650 3050 2650
Wire Wire Line
	850  2650 1100 2650
Text Label 3350 3150 2    50   ~ 0
POW4_in
Wire Wire Line
	2950 3150 3350 3150
Connection ~ 2950 3150
Text Label 3350 2950 2    50   ~ 0
VDD18_in
Wire Wire Line
	2950 2950 3350 2950
Wire Wire Line
	2950 2850 2950 2950
Connection ~ 2950 2950
Text Label 5000 3050 0    50   ~ 0
VDD18_in
Text Label 5000 3250 0    50   ~ 0
POW4_in
Wire Wire Line
	3450 2500 3450 3250
$Comp
L astropix:subd #PWR0119
U 1 1 60F171C4
P 6300 1300
F 0 "#PWR0119" H 6250 1100 50  0001 C CNN
F 1 "subd" H 6305 1127 50  0000 C CNN
F 2 "" H 6300 1300 50  0001 C CNN
F 3 "" H 6300 1300 50  0001 C CNN
	1    6300 1300
	-1   0    0    -1  
$EndComp
$Comp
L astropix:suba #PWR0120
U 1 1 60F1B1CC
P 6300 2200
F 0 "#PWR0120" H 6250 2000 50  0001 C CNN
F 1 "suba" H 6305 2027 50  0000 C CNN
F 2 "" H 6300 2200 50  0001 C CNN
F 3 "" H 6300 2200 50  0001 C CNN
	1    6300 2200
	-1   0    0    -1  
$EndComp
$Comp
L astropix:-HV #PWR0121
U 1 1 60F232A1
P 4650 1300
F 0 "#PWR0121" H 4600 1100 50  0001 C CNN
F 1 "-HV" H 4655 1127 50  0000 C CNN
F 2 "" H 4650 1300 50  0001 C CNN
F 3 "" H 4650 1300 50  0001 C CNN
	1    4650 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 1300 4650 1250
$Comp
L Device:R R1
U 1 1 60F37D59
P 5050 1250
F 0 "R1" V 4843 1250 50  0000 C CNN
F 1 "1k" V 4934 1250 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4980 1250 50  0001 C CNN
F 3 "~" H 5050 1250 50  0001 C CNN
	1    5050 1250
	0    1    1    0   
$EndComp
Wire Wire Line
	4900 1250 4650 1250
$Comp
L Device:C C3
U 1 1 60F46193
P 5650 1000
F 0 "C3" H 5765 1046 50  0000 L CNN
F 1 ".1uF" H 5765 955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5688 850 50  0001 C CNN
F 3 "~" H 5650 1000 50  0001 C CNN
	1    5650 1000
	1    0    0    -1  
$EndComp
$Comp
L Device:C C5
U 1 1 60F4ADC0
P 6050 1000
F 0 "C5" H 6165 1046 50  0000 L CNN
F 1 ".1uF" H 6165 955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6088 850 50  0001 C CNN
F 3 "~" H 6050 1000 50  0001 C CNN
	1    6050 1000
	1    0    0    -1  
$EndComp
$Comp
L Device:C C4
U 1 1 60F53D5F
P 5650 1900
F 0 "C4" H 5765 1946 50  0000 L CNN
F 1 ".1uF" H 5765 1855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5688 1750 50  0001 C CNN
F 3 "~" H 5650 1900 50  0001 C CNN
	1    5650 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:C C6
U 1 1 60F53D65
P 6050 1900
F 0 "C6" H 6165 1946 50  0000 L CNN
F 1 ".1uF" H 6165 1855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6088 1750 50  0001 C CNN
F 3 "~" H 6050 1900 50  0001 C CNN
	1    6050 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 2150 5650 2050
Wire Wire Line
	6050 2150 6050 2050
Wire Wire Line
	5650 1250 5650 1150
Wire Wire Line
	6050 1250 6050 1150
Wire Wire Line
	5200 1250 5350 1250
Wire Wire Line
	6300 1250 6300 1300
Wire Wire Line
	5350 1250 5350 1800
Wire Wire Line
	6300 2200 6300 2150
Wire Wire Line
	5650 750  5650 850 
Wire Wire Line
	6050 750  6050 850 
$Comp
L power:GNDA #PWR0122
U 1 1 60FAF741
P 6300 1650
F 0 "#PWR0122" H 6300 1400 50  0001 C CNN
F 1 "GNDA" H 6305 1477 50  0000 C CNN
F 2 "" H 6300 1650 50  0001 C CNN
F 3 "" H 6300 1650 50  0001 C CNN
	1    6300 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 1750 5650 1650
Wire Wire Line
	6050 1750 6050 1650
Wire Wire Line
	6050 1650 6300 1650
$Comp
L power:GNDD #PWR0123
U 1 1 60FCDA87
P 6300 750
F 0 "#PWR0123" H 6300 500 50  0001 C CNN
F 1 "GNDD" H 6304 595 50  0000 C CNN
F 2 "" H 6300 750 50  0001 C CNN
F 3 "" H 6300 750 50  0001 C CNN
	1    6300 750 
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6050 2150 6300 2150
Connection ~ 6050 2150
Wire Wire Line
	6050 1250 6300 1250
Connection ~ 6050 1250
Wire Wire Line
	6300 750  6050 750 
Connection ~ 5350 1250
$Comp
L Device:C C12
U 1 1 6103FCDB
P 8250 7350
F 0 "C12" H 8365 7396 50  0000 L CNN
F 1 ".1uF" H 8365 7305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8288 7200 50  0001 C CNN
F 3 "~" H 8250 7350 50  0001 C CNN
	1    8250 7350
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR0124
U 1 1 61056058
P 8250 7600
F 0 "#PWR0124" H 8250 7350 50  0001 C CNN
F 1 "GNDD" H 8254 7445 50  0000 C CNN
F 2 "" H 8250 7600 50  0001 C CNN
F 3 "" H 8250 7600 50  0001 C CNN
	1    8250 7600
	-1   0    0    -1  
$EndComp
$Comp
L Device:C C9
U 1 1 6107E1BB
P 2900 1300
F 0 "C9" H 3015 1346 50  0000 L CNN
F 1 "1nF" H 3015 1255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2938 1150 50  0001 C CNN
F 3 "~" H 2900 1300 50  0001 C CNN
	1    2900 1300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C10
U 1 1 6107E1C1
P 3300 1300
F 0 "C10" H 3415 1346 50  0000 L CNN
F 1 ".1uF" H 3415 1255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3338 1150 50  0001 C CNN
F 3 "~" H 3300 1300 50  0001 C CNN
	1    3300 1300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C11
U 1 1 6107E1C7
P 3700 1300
F 0 "C11" H 3815 1346 50  0000 L CNN
F 1 "10uF" H 3815 1255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3738 1150 50  0001 C CNN
F 3 "~" H 3700 1300 50  0001 C CNN
	1    3700 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 1450 2900 1550
Wire Wire Line
	3300 1550 3300 1450
Wire Wire Line
	3700 1550 3700 1450
Wire Wire Line
	2900 1050 2900 1150
Wire Wire Line
	3300 1050 3300 1150
Wire Wire Line
	3700 1050 3700 1150
Wire Wire Line
	2750 950  2750 1050
Wire Wire Line
	2750 1050 2900 1050
Wire Wire Line
	2750 1600 2750 1550
Wire Wire Line
	2750 1550 2900 1550
$Comp
L power:VDD #PWR0125
U 1 1 610A15AF
P 5400 2850
F 0 "#PWR0125" H 5400 2700 50  0001 C CNN
F 1 "VDD" H 5415 3023 50  0000 C CNN
F 2 "" H 5400 2850 50  0001 C CNN
F 3 "" H 5400 2850 50  0001 C CNN
	1    5400 2850
	-1   0    0    -1  
$EndComp
$Comp
L power:VDDA #PWR0126
U 1 1 610D52A7
P 8700 1150
F 0 "#PWR0126" H 8700 1000 50  0001 C CNN
F 1 "VDDA" H 8715 1323 50  0000 C CNN
F 2 "" H 8700 1150 50  0001 C CNN
F 3 "" H 8700 1150 50  0001 C CNN
	1    8700 1150
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR0127
U 1 1 610DDAF1
P 9900 1950
F 0 "#PWR0127" H 9900 1700 50  0001 C CNN
F 1 "GNDA" H 9905 1777 50  0000 C CNN
F 2 "" H 9900 1950 50  0001 C CNN
F 3 "" H 9900 1950 50  0001 C CNN
	1    9900 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 1500 9700 1500
Wire Wire Line
	8250 7500 8250 7600
Wire Wire Line
	8250 7100 8250 7200
$Comp
L Device:R R3
U 1 1 6112F08F
P 8100 8150
F 0 "R3" H 8170 8196 50  0000 L CNN
F 1 "100" H 8170 8105 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 8030 8150 50  0001 C CNN
F 3 "~" H 8100 8150 50  0001 C CNN
	1    8100 8150
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 8050 8250 8050
Wire Wire Line
	8100 8350 8250 8350
Wire Wire Line
	8250 8350 8250 8250
Wire Wire Line
	8250 8250 8350 8250
Wire Wire Line
	8100 7950 8100 8000
Wire Wire Line
	8100 7950 8250 7950
Wire Wire Line
	8250 7950 8250 8050
Wire Wire Line
	8100 8300 8100 8350
Text Label 7350 8350 0    50   ~ 0
sample_clock_se_N
Text Label 7350 7950 0    50   ~ 0
sample_clock_se_P
Wire Wire Line
	7350 8350 8100 8350
Connection ~ 8100 8350
Wire Wire Line
	7350 7950 8100 7950
Connection ~ 8100 7950
$Comp
L astropix:pcie_con J1
U 1 1 61228AD2
P 2000 4900
F 0 "J1" H 1975 7375 50  0000 C CNN
F 1 "pcie_con" H 1975 7284 50  0000 C CNN
F 2 "astropix:pcie_astropix" H 2600 4900 50  0001 C CNN
F 3 "" H 2600 4900 50  0001 C CNN
	1    2000 4900
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:OPA340NA U3
U 1 1 60D0841F
P 8800 2950
F 0 "U3" H 8450 3250 50  0000 L CNN
F 1 "OPA320" H 8850 2800 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 8700 2750 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/opa340.pdf" H 8800 3150 50  0001 C CNN
	1    8800 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8700 2600 8700 2650
$Comp
L power:GNDA #PWR0128
U 1 1 60D08426
P 8700 3400
F 0 "#PWR0128" H 8700 3150 50  0001 C CNN
F 1 "GNDA" H 8705 3227 50  0000 C CNN
F 2 "" H 8700 3400 50  0001 C CNN
F 3 "" H 8700 3400 50  0001 C CNN
	1    8700 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8700 3250 8700 3400
Wire Wire Line
	8500 3050 8450 3050
Wire Wire Line
	8450 3050 8450 3300
Wire Wire Line
	9150 3300 9150 2950
Wire Wire Line
	9150 2950 9100 2950
Wire Wire Line
	8450 3300 9150 3300
$Comp
L Device:R R6
U 1 1 60D08432
P 9450 2950
F 0 "R6" V 9243 2950 50  0000 C CNN
F 1 "22" V 9334 2950 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 9380 2950 50  0001 C CNN
F 3 "~" H 9450 2950 50  0001 C CNN
	1    9450 2950
	0    1    1    0   
$EndComp
Wire Wire Line
	9150 2950 9300 2950
Connection ~ 9150 2950
$Comp
L Connector:Conn_Coaxial J4
U 1 1 60D0843A
P 9900 2950
F 0 "J4" H 10000 2925 50  0000 L CNN
F 1 "SFOut" H 10000 2834 50  0000 L CNN
F 2 "Connector_Coaxial:SMA_Molex_73251-2200_Horizontal" H 9900 2950 50  0001 C CNN
F 3 " ~" H 9900 2950 50  0001 C CNN
	1    9900 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 3150 9900 3400
$Comp
L power:VDDA #PWR0129
U 1 1 60D08441
P 8700 2600
F 0 "#PWR0129" H 8700 2450 50  0001 C CNN
F 1 "VDDA" H 8715 2773 50  0000 C CNN
F 2 "" H 8700 2600 50  0001 C CNN
F 3 "" H 8700 2600 50  0001 C CNN
	1    8700 2600
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR0130
U 1 1 60D08447
P 9900 3400
F 0 "#PWR0130" H 9900 3150 50  0001 C CNN
F 1 "GNDA" H 9905 3227 50  0000 C CNN
F 2 "" H 9900 3400 50  0001 C CNN
F 3 "" H 9900 3400 50  0001 C CNN
	1    9900 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 2950 9700 2950
$Comp
L Device:C C7
U 1 1 60D1FE26
P 1400 1300
F 0 "C7" H 1515 1346 50  0000 L CNN
F 1 "1n" H 1515 1255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 1438 1150 50  0001 C CNN
F 3 "~" H 1400 1300 50  0001 C CNN
	1    1400 1300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C8
U 1 1 60D1FE2C
P 1800 1300
F 0 "C8" H 1915 1346 50  0000 L CNN
F 1 ".1uF" H 1915 1255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 1838 1150 50  0001 C CNN
F 3 "~" H 1800 1300 50  0001 C CNN
	1    1800 1300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C13
U 1 1 60D1FE32
P 2200 1300
F 0 "C13" H 2315 1346 50  0000 L CNN
F 1 "10uF" H 2315 1255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2238 1150 50  0001 C CNN
F 3 "~" H 2200 1300 50  0001 C CNN
	1    2200 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1400 1450 1400 1550
Wire Wire Line
	1800 1550 1800 1450
Wire Wire Line
	2200 1550 2200 1450
Wire Wire Line
	1250 1600 1250 1550
Wire Wire Line
	1250 1550 1400 1550
$Comp
L power:GNDD #PWR0131
U 1 1 60D41363
P 1250 1600
F 0 "#PWR0131" H 1250 1350 50  0001 C CNN
F 1 "GNDD" H 1254 1445 50  0000 C CNN
F 2 "" H 1250 1600 50  0001 C CNN
F 3 "" H 1250 1600 50  0001 C CNN
	1    1250 1600
	-1   0    0    -1  
$EndComp
$Comp
L power:VDDA #PWR0133
U 1 1 60D57A7F
P 11750 750
F 0 "#PWR0133" H 11750 600 50  0001 C CNN
F 1 "VDDA" H 11765 923 50  0000 C CNN
F 2 "" H 11750 750 50  0001 C CNN
F 3 "" H 11750 750 50  0001 C CNN
	1    11750 750 
	1    0    0    -1  
$EndComp
Wire Wire Line
	11600 2350 11000 2350
Text Label 11000 2350 0    50   ~ 0
Inj
Text Label 11000 2550 0    50   ~ 0
DAC_blpix
Text Label 11000 2650 0    50   ~ 0
DAC_VCasc2!
Text Label 11000 2750 0    50   ~ 0
DAC_ThPix
Text Label 11000 2850 0    50   ~ 0
DAC_vminuspix
Wire Wire Line
	11000 2850 11600 2850
Wire Wire Line
	11000 2750 11600 2750
Wire Wire Line
	11000 2650 11600 2650
Wire Wire Line
	11000 2550 11600 2550
$Comp
L Device:R R7
U 1 1 60E4745D
P 14400 6150
F 0 "R7" H 14470 6196 50  0000 L CNN
F 1 "10k" H 14470 6105 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 14330 6150 50  0001 C CNN
F 3 "~" H 14400 6150 50  0001 C CNN
	1    14400 6150
	-1   0    0    -1  
$EndComp
Wire Wire Line
	14400 6300 14400 6350
Connection ~ 14400 6350
Wire Wire Line
	14400 6350 14800 6350
$Comp
L power:VDD #PWR0135
U 1 1 60E47466
P 14400 5950
F 0 "#PWR0135" H 14400 5800 50  0001 C CNN
F 1 "VDD" H 14415 6123 50  0000 C CNN
F 2 "" H 14400 5950 50  0001 C CNN
F 3 "" H 14400 5950 50  0001 C CNN
	1    14400 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	14400 6000 14400 5950
Text Label 14800 6350 2    50   ~ 0
interrupt
Wire Wire Line
	14400 6350 14050 6350
Wire Wire Line
	9400 8150 10350 8150
Wire Wire Line
	10350 6950 10350 8150
Text Label 11000 8150 0    50   ~ 0
spi_left_miso1
Text Label 11000 8450 0    50   ~ 0
spi_left_clk
Text Label 11000 8550 0    50   ~ 0
spi_left_csn
Text Label 11000 8250 0    50   ~ 0
spi_left_miso0
Text Label 11000 8350 0    50   ~ 0
spi_left_mosi
Wire Wire Line
	11000 8150 11650 8150
Wire Wire Line
	11000 8250 11650 8250
Wire Wire Line
	11000 8350 11650 8350
Wire Wire Line
	11000 8450 11650 8450
Wire Wire Line
	11000 8550 11650 8550
Text Label 14700 8150 2    50   ~ 0
spi_right_miso1
Text Label 14700 8450 2    50   ~ 0
spi_right_clk
Text Label 14700 8550 2    50   ~ 0
spi_right_csn
Text Label 14700 8250 2    50   ~ 0
spi_right_miso0
Text Label 14700 8350 2    50   ~ 0
spi_right_mosi
Wire Wire Line
	14700 8150 14050 8150
Wire Wire Line
	14700 8250 14050 8250
Wire Wire Line
	14700 8350 14050 8350
Wire Wire Line
	14700 8450 14050 8450
Wire Wire Line
	14700 8550 14050 8550
Text Label 14650 6950 2    50   ~ 0
sc_out
Text Label 10250 8150 2    50   ~ 0
sample_clock_se_in
$Comp
L Connector:Conn_Coaxial J5
U 1 1 60F540B4
P 14700 7450
F 0 "J5" H 14800 7425 50  0000 L CNN
F 1 "sample_clock_out" H 14800 7334 50  0000 L CNN
F 2 "Connector_Coaxial:SMA_Molex_73251-2200_Horizontal" H 14700 7450 50  0001 C CNN
F 3 " ~" H 14700 7450 50  0001 C CNN
	1    14700 7450
	1    0    0    -1  
$EndComp
Wire Wire Line
	14700 7650 14700 7750
$Comp
L Mechanical:MountingHole H1
U 1 1 60FBE07F
P 1500 9700
F 0 "H1" H 1600 9746 50  0000 L CNN
F 1 "MountingHole" H 1600 9655 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 1500 9700 50  0001 C CNN
F 3 "~" H 1500 9700 50  0001 C CNN
	1    1500 9700
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H3
U 1 1 60FBF783
P 2200 9700
F 0 "H3" H 2300 9746 50  0000 L CNN
F 1 "MountingHole" H 2300 9655 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 2200 9700 50  0001 C CNN
F 3 "~" H 2200 9700 50  0001 C CNN
	1    2200 9700
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H2
U 1 1 60FCD068
P 1500 10100
F 0 "H2" H 1600 10146 50  0000 L CNN
F 1 "MountingHole" H 1600 10055 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 1500 10100 50  0001 C CNN
F 3 "~" H 1500 10100 50  0001 C CNN
	1    1500 10100
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H4
U 1 1 60FDA9D1
P 2200 10100
F 0 "H4" H 2300 10146 50  0000 L CNN
F 1 "MountingHole" H 2300 10055 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 2200 10100 50  0001 C CNN
F 3 "~" H 2200 10100 50  0001 C CNN
	1    2200 10100
	1    0    0    -1  
$EndComp
Text Label 14800 6450 2    50   ~ 0
hold
Wire Wire Line
	14800 6450 14050 6450
$Comp
L power:GNDA #PWR0137
U 1 1 60CF666E
P 5500 3700
F 0 "#PWR0137" H 5500 3450 50  0001 C CNN
F 1 "GNDA" H 5505 3527 50  0000 C CNN
F 2 "" H 5500 3700 50  0001 C CNN
F 3 "" H 5500 3700 50  0001 C CNN
	1    5500 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 3700 5500 3650
Wire Wire Line
	4950 3700 4950 3650
$Comp
L power:GNDD #PWR0138
U 1 1 60CF6676
P 4950 3700
F 0 "#PWR0138" H 4950 3450 50  0001 C CNN
F 1 "GNDD" H 4954 3545 50  0000 C CNN
F 2 "" H 4950 3700 50  0001 C CNN
F 3 "" H 4950 3700 50  0001 C CNN
	1    4950 3700
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR0139
U 1 1 60D128CF
P 5250 3700
F 0 "#PWR0139" H 5250 3450 50  0001 C CNN
F 1 "GND" H 5255 3527 50  0000 C CNN
F 2 "" H 5250 3700 50  0001 C CNN
F 3 "" H 5250 3700 50  0001 C CNN
	1    5250 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 3650 5250 3650
Wire Wire Line
	5250 3700 5250 3650
Connection ~ 5250 3650
Wire Wire Line
	5250 3650 5500 3650
Wire Wire Line
	1400 1550 1800 1550
Connection ~ 1400 1550
Wire Wire Line
	1800 1550 2200 1550
Connection ~ 1800 1550
Connection ~ 1400 1050
Wire Wire Line
	1400 1050 1800 1050
Connection ~ 1800 1050
Wire Wire Line
	1800 1050 2200 1050
$Comp
L power:VDD #PWR0132
U 1 1 60D4C4CE
P 1250 950
F 0 "#PWR0132" H 1250 800 50  0001 C CNN
F 1 "VDD" H 1265 1123 50  0000 C CNN
F 2 "" H 1250 950 50  0001 C CNN
F 3 "" H 1250 950 50  0001 C CNN
	1    1250 950 
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1250 1050 1400 1050
Wire Wire Line
	1250 950  1250 1050
Wire Wire Line
	2200 1050 2200 1150
Wire Wire Line
	1800 1050 1800 1150
Wire Wire Line
	1400 1050 1400 1150
Wire Wire Line
	2900 1050 3300 1050
Connection ~ 2900 1050
Wire Wire Line
	2900 1550 3300 1550
Connection ~ 2900 1550
Wire Wire Line
	3300 1050 3700 1050
Connection ~ 3300 1050
Wire Wire Line
	3300 1550 3700 1550
Connection ~ 3300 1550
Wire Wire Line
	2850 4550 3550 4550
Text Label 3550 4550 2    50   ~ 0
res_n
Wire Wire Line
	2850 4450 3550 4450
Text Label 3550 4450 2    50   ~ 0
interrupt
Text Label 3450 5850 2    50   ~ 0
DAC_blpix
Text Label 3450 5950 2    50   ~ 0
DAC_VCasc2!
Text Label 3450 6050 2    50   ~ 0
DAC_ThPix
Text Label 3450 6150 2    50   ~ 0
DAC_vminuspix
Wire Wire Line
	3450 6150 2850 6150
Wire Wire Line
	3450 6050 2850 6050
Wire Wire Line
	3450 5950 2850 5950
Wire Wire Line
	3450 5850 2850 5850
NoConn ~ 2850 6650
NoConn ~ 2850 6750
NoConn ~ 2850 6850
NoConn ~ 2850 6950
NoConn ~ 2850 7050
NoConn ~ 2850 7150
NoConn ~ 2850 7250
NoConn ~ 2850 7350
NoConn ~ 2850 7450
NoConn ~ 2850 7550
NoConn ~ 1100 7550
NoConn ~ 1100 7450
NoConn ~ 1100 7350
NoConn ~ 1100 7250
NoConn ~ 1100 7150
NoConn ~ 1100 7050
NoConn ~ 1100 6950
NoConn ~ 1100 6850
NoConn ~ 1100 6750
NoConn ~ 1100 6650
NoConn ~ 1100 5450
NoConn ~ 1100 5550
NoConn ~ 1100 5650
NoConn ~ 1100 5750
Connection ~ 11100 6500
Text Label 10450 6500 0    50   ~ 0
sample_clock_P
Wire Wire Line
	11100 6500 10450 6500
Text Label 3550 4350 2    50   ~ 0
sample_clock_N
Wire Wire Line
	3550 4350 2850 4350
Text Label 3550 4250 2    50   ~ 0
sample_clock_P
Wire Wire Line
	2850 4250 3550 4250
Text Label 3550 4650 2    50   ~ 0
timestamp_clk
Wire Wire Line
	3550 4650 2850 4650
Text Label 3550 4750 2    50   ~ 0
hold
Wire Wire Line
	3550 4750 2850 4750
$Comp
L Device:R R9
U 1 1 611D4015
P 10700 8000
F 0 "R9" H 10770 8046 50  0000 L CNN
F 1 "10k" H 10770 7955 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 10630 8000 50  0001 C CNN
F 3 "~" H 10700 8000 50  0001 C CNN
	1    10700 8000
	1    0    0    -1  
$EndComp
Wire Wire Line
	10700 7850 10700 7800
Wire Wire Line
	10700 7800 11650 7800
$Comp
L power:GNDD #PWR0140
U 1 1 611F5306
P 10700 8200
F 0 "#PWR0140" H 10700 7950 50  0001 C CNN
F 1 "GNDD" H 10704 8045 50  0000 C CNN
F 2 "" H 10700 8200 50  0001 C CNN
F 3 "" H 10700 8200 50  0001 C CNN
	1    10700 8200
	-1   0    0    -1  
$EndComp
Wire Wire Line
	10700 8200 10700 8150
NoConn ~ 1100 5250
NoConn ~ 1100 5350
NoConn ~ 2850 5250
NoConn ~ 2850 5350
Wire Wire Line
	1100 6250 800  6250
Text Label 800  6250 0    50   ~ 0
Inj
NoConn ~ 1100 6350
NoConn ~ 1100 6450
NoConn ~ 1100 6550
NoConn ~ 2850 6550
NoConn ~ 2850 6450
NoConn ~ 2850 6350
NoConn ~ 2850 6250
NoConn ~ 1100 6150
Connection ~ 10800 6100
Wire Wire Line
	10800 6100 10450 6100
Wire Wire Line
	10800 6100 11650 6100
Text Label 14600 3100 2    50   ~ 0
SFOut
Wire Wire Line
	14600 3100 14000 3100
NoConn ~ 2850 5450
NoConn ~ 2850 5550
NoConn ~ 2850 5650
NoConn ~ 2850 5750
NoConn ~ 2850 4150
NoConn ~ 2850 4050
NoConn ~ 2850 3950
NoConn ~ 2850 3850
NoConn ~ 1100 4150
NoConn ~ 1100 4050
NoConn ~ 1100 3950
NoConn ~ 1100 3850
Connection ~ 12950 4550
Wire Wire Line
	12950 4550 13100 4550
Wire Wire Line
	14000 3450 14300 3450
Text Label 14300 3450 2    50   ~ 0
Vtemp1
Wire Wire Line
	8500 1400 8200 1400
Text Label 8200 1400 0    50   ~ 0
Vtemp1
Wire Wire Line
	8500 2850 8200 2850
Text Label 8200 2850 0    50   ~ 0
SFOut
$Comp
L Connector_Generic:Conn_01x03 J8
U 1 1 60D156C8
P 14550 2100
F 0 "J8" H 14468 2417 50  0000 C CNN
F 1 "Conn_01x03" H 14468 2326 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 14550 2100 50  0001 C CNN
F 3 "~" H 14550 2100 50  0001 C CNN
	1    14550 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	14350 2000 14000 2000
Wire Wire Line
	14000 2100 14350 2100
$Comp
L power:GNDA #PWR0142
U 1 1 60D3AC50
P 14250 2250
F 0 "#PWR0142" H 14250 2000 50  0001 C CNN
F 1 "GNDA" H 14255 2077 50  0000 C CNN
F 2 "" H 14250 2250 50  0001 C CNN
F 3 "" H 14250 2250 50  0001 C CNN
	1    14250 2250
	-1   0    0    -1  
$EndComp
Wire Wire Line
	14350 2200 14250 2200
Wire Wire Line
	14250 2200 14250 2250
$Comp
L Connector_Generic:Conn_01x03 J7
U 1 1 60D60B6F
P 15000 6450
F 0 "J7" H 14918 6767 50  0000 C CNN
F 1 "Conn_01x03" H 14918 6676 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 15000 6450 50  0001 C CNN
F 3 "~" H 15000 6450 50  0001 C CNN
	1    15000 6450
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR0143
U 1 1 60DAA034
P 14750 6600
F 0 "#PWR0143" H 14750 6350 50  0001 C CNN
F 1 "GNDD" H 14754 6445 50  0000 C CNN
F 2 "" H 14750 6600 50  0001 C CNN
F 3 "" H 14750 6600 50  0001 C CNN
	1    14750 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	14800 6550 14750 6550
Wire Wire Line
	14750 6550 14750 6600
Text Label 8150 9950 0    50   ~ 0
spi_left_miso1
Text Label 8150 10250 0    50   ~ 0
spi_left_clk
Text Label 8150 10350 0    50   ~ 0
spi_left_csn
Text Label 8150 10050 0    50   ~ 0
spi_left_miso0
Text Label 8150 10150 0    50   ~ 0
spi_left_mosi
Wire Wire Line
	8150 9950 8800 9950
Wire Wire Line
	8150 10050 8800 10050
Wire Wire Line
	8150 10150 8800 10150
Wire Wire Line
	8150 10250 8800 10250
Wire Wire Line
	8150 10350 8800 10350
$Comp
L Connector_Generic:Conn_01x06 J9
U 1 1 60E22831
P 9000 10150
F 0 "J9" H 9080 10142 50  0000 L CNN
F 1 "SPI_left" H 9080 10051 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 9000 10150 50  0001 C CNN
F 3 "~" H 9000 10150 50  0001 C CNN
	1    9000 10150
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR0141
U 1 1 60E23BB1
P 8700 10500
F 0 "#PWR0141" H 8700 10250 50  0001 C CNN
F 1 "GNDD" H 8704 10345 50  0000 C CNN
F 2 "" H 8700 10500 50  0001 C CNN
F 3 "" H 8700 10500 50  0001 C CNN
	1    8700 10500
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8700 10500 8700 10450
Wire Wire Line
	8700 10450 8800 10450
$Comp
L Connector_Generic:Conn_01x06 J10
U 1 1 60E4B7FE
P 10700 10150
F 0 "J10" H 10780 10142 50  0000 L CNN
F 1 "SPI_right" H 10780 10051 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 10700 10150 50  0001 C CNN
F 3 "~" H 10700 10150 50  0001 C CNN
	1    10700 10150
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR0144
U 1 1 60E4B804
P 10400 10500
F 0 "#PWR0144" H 10400 10250 50  0001 C CNN
F 1 "GNDD" H 10404 10345 50  0000 C CNN
F 2 "" H 10400 10500 50  0001 C CNN
F 3 "" H 10400 10500 50  0001 C CNN
	1    10400 10500
	-1   0    0    -1  
$EndComp
Wire Wire Line
	10400 10500 10400 10450
Wire Wire Line
	10400 10450 10500 10450
Text Label 9850 9950 0    50   ~ 0
spi_right_miso1
Text Label 9850 10250 0    50   ~ 0
spi_right_clk
Text Label 9850 10350 0    50   ~ 0
spi_right_csn
Text Label 9850 10050 0    50   ~ 0
spi_right_miso0
Text Label 9850 10150 0    50   ~ 0
spi_right_mosi
Wire Wire Line
	9850 9950 10500 9950
Wire Wire Line
	9850 10050 10500 10050
Wire Wire Line
	9850 10150 10500 10150
Wire Wire Line
	9850 10250 10500 10250
Wire Wire Line
	9850 10350 10500 10350
$Comp
L Connector:Conn_Coaxial J6
U 1 1 60F7DBC9
P 4650 1800
F 0 "J6" H 4750 1775 50  0000 L CNN
F 1 "HV_in_external" H 4750 1684 50  0000 L CNN
F 2 "Connector_Coaxial:SMA_Molex_73251-2200_Horizontal" H 4650 1800 50  0001 C CNN
F 3 " ~" H 4650 1800 50  0001 C CNN
	1    4650 1800
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4650 2000 4650 2100
$Comp
L power:GNDA #PWR0145
U 1 1 60F7DBD0
P 4650 2100
F 0 "#PWR0145" H 4650 1850 50  0001 C CNN
F 1 "GNDA" H 4655 1927 50  0000 C CNN
F 2 "" H 4650 2100 50  0001 C CNN
F 3 "" H 4650 2100 50  0001 C CNN
	1    4650 2100
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R10
U 1 1 6100BAE2
P 5100 1800
F 0 "R10" V 4893 1800 50  0000 C CNN
F 1 "1k" V 4984 1800 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5030 1800 50  0001 C CNN
F 3 "~" H 5100 1800 50  0001 C CNN
	1    5100 1800
	0    1    1    0   
$EndComp
Wire Wire Line
	4850 1800 4950 1800
Wire Wire Line
	5250 1800 5350 1800
Connection ~ 5350 1800
Wire Wire Line
	5350 1800 5350 2150
Wire Wire Line
	5350 2150 5650 2150
Wire Wire Line
	5350 1250 5650 1250
Connection ~ 5650 1250
Wire Wire Line
	5650 1250 6050 1250
Connection ~ 5650 2150
Wire Wire Line
	5650 2150 6050 2150
Wire Wire Line
	5650 1650 6050 1650
Connection ~ 6050 1650
Wire Wire Line
	5650 750  6050 750 
Connection ~ 6050 750 
$Comp
L power:VDD #PWR0146
U 1 1 611F9AB5
P 7800 2800
F 0 "#PWR0146" H 7800 2650 50  0001 C CNN
F 1 "VDD" H 7815 2973 50  0000 C CNN
F 2 "" H 7800 2800 50  0001 C CNN
F 3 "" H 7800 2800 50  0001 C CNN
	1    7800 2800
	-1   0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 611F9ABB
P 7800 3050
F 0 "C2" H 7915 3096 50  0000 L CNN
F 1 ".1uF" H 7915 3005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7838 2900 50  0001 C CNN
F 3 "~" H 7800 3050 50  0001 C CNN
	1    7800 3050
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR0147
U 1 1 611F9AC1
P 7800 3300
F 0 "#PWR0147" H 7800 3050 50  0001 C CNN
F 1 "GNDD" H 7804 3145 50  0000 C CNN
F 2 "" H 7800 3300 50  0001 C CNN
F 3 "" H 7800 3300 50  0001 C CNN
	1    7800 3300
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7800 3200 7800 3300
Wire Wire Line
	7800 2800 7800 2900
$Comp
L power:VDD #PWR0148
U 1 1 6120E196
P 7800 1350
F 0 "#PWR0148" H 7800 1200 50  0001 C CNN
F 1 "VDD" H 7815 1523 50  0000 C CNN
F 2 "" H 7800 1350 50  0001 C CNN
F 3 "" H 7800 1350 50  0001 C CNN
	1    7800 1350
	-1   0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 6120E19C
P 7800 1600
F 0 "C1" H 7915 1646 50  0000 L CNN
F 1 ".1uF" H 7915 1555 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7838 1450 50  0001 C CNN
F 3 "~" H 7800 1600 50  0001 C CNN
	1    7800 1600
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR0149
U 1 1 6120E1A2
P 7800 1850
F 0 "#PWR0149" H 7800 1600 50  0001 C CNN
F 1 "GNDD" H 7804 1695 50  0000 C CNN
F 2 "" H 7800 1850 50  0001 C CNN
F 3 "" H 7800 1850 50  0001 C CNN
	1    7800 1850
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7800 1750 7800 1850
Wire Wire Line
	7800 1350 7800 1450
$Comp
L astropix:AstroPix2_split U4
U 2 1 6127B119
P 12850 6650
F 0 "U4" H 11950 8050 50  0000 C CNN
F 1 "AstroPix2_split" H 13900 3900 50  0000 C CNN
F 2 "astropix:AstroPix2" H 12250 8350 50  0001 C CNN
F 3 "" H 12000 8350 50  0001 C CNN
	2    12850 6650
	1    0    0    -1  
$EndComp
Wire Wire Line
	12650 900  12650 750 
Wire Wire Line
	12750 900  12750 750 
Wire Wire Line
	12350 4400 12350 4550
Wire Wire Line
	12450 4400 12450 4550
Wire Wire Line
	12550 4400 12550 4550
$Comp
L power:GNDA #PWR0134
U 1 1 613A06FF
P 12000 4600
F 0 "#PWR0134" H 12000 4350 50  0001 C CNN
F 1 "GNDA" H 12005 4427 50  0000 C CNN
F 2 "" H 12000 4600 50  0001 C CNN
F 3 "" H 12000 4600 50  0001 C CNN
	1    12000 4600
	-1   0    0    -1  
$EndComp
Wire Wire Line
	12000 4600 12000 4400
Wire Wire Line
	12950 4400 12950 4550
Wire Wire Line
	13100 4400 13100 4550
Wire Wire Line
	13200 4400 13200 4550
Wire Wire Line
	13300 4400 13300 4550
Wire Wire Line
	13400 4400 13400 4550
Wire Wire Line
	13500 4400 13500 4550
Wire Wire Line
	12800 5100 12800 5200
Wire Wire Line
	14050 6950 14650 6950
Wire Wire Line
	12550 9450 12550 9600
Wire Wire Line
	12650 9450 12650 9600
Wire Wire Line
	12550 9650 12550 9600
$Comp
L power:GNDD #PWR0109
U 1 1 60DA236F
P 12550 9650
F 0 "#PWR0109" H 12550 9400 50  0001 C CNN
F 1 "GNDD" H 12554 9495 50  0000 C CNN
F 2 "" H 12550 9650 50  0001 C CNN
F 3 "" H 12550 9650 50  0001 C CNN
	1    12550 9650
	-1   0    0    -1  
$EndComp
Wire Wire Line
	14050 7450 14500 7450
$Comp
L power:GNDD #PWR0136
U 1 1 6168FAAD
P 14700 7750
F 0 "#PWR0136" H 14700 7500 50  0001 C CNN
F 1 "GNDD" H 14704 7595 50  0000 C CNN
F 2 "" H 14700 7750 50  0001 C CNN
F 3 "" H 14700 7750 50  0001 C CNN
	1    14700 7750
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 60D6E0DF
P 12000 800
F 0 "R8" V 11793 800 50  0000 C CNN
F 1 "0" V 11884 800 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 11930 800 50  0001 C CNN
F 3 "~" H 12000 800 50  0001 C CNN
	1    12000 800 
	0    1    1    0   
$EndComp
$Comp
L astropix:AstroPix2_split U4
U 1 1 6127915C
P 12800 2550
F 0 "U4" H 11800 4100 50  0000 L CNN
F 1 "AstroPix2_split" H 13800 800 50  0000 L CNN
F 2 "astropix:AstroPix2" H 12200 4250 50  0001 C CNN
F 3 "" H 11950 4250 50  0001 C CNN
	1    12800 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	12250 800  12250 900 
Wire Wire Line
	12150 800  12250 800 
Wire Wire Line
	11750 750  11750 800 
Wire Wire Line
	11750 800  11850 800 
Wire Wire Line
	12900 5200 12900 5100
Wire Wire Line
	10350 6950 11650 6950
$EndSCHEMATC
