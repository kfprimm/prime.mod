
Strict

Module sys87.CgToolkit
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"

Import "-lCg"
Import "cggl.bmx"

Const CG_VERSION_NUM = 3000

Const CG_FALSE = 0
Const CG_TRUE  = 1

' CGbehavior
Const CG_BEHAVIOR_UNKNOWN = 0
Const CG_BEHAVIOR_LATEST  = 1
Const CG_BEHAVIOR_2200    = 1000
Const CG_BEHAVIOR_3000    = 2000
Const CG_BEHAVIOR_CURRENT = CG_BEHAVIOR_3000

' CGtype
Const CG_UNKNOWN_TYPE      = 0
Const CG_ARRAY             = 2
Const CG_STRING            = 1135
Const CG_STRUCT            = 1
Const CG_TYPELESS_STRUCT   = 3
Const CG_TEXTURE           = 1137
Const CG_PIXELSHADER_TYPE  = 1142
Const CG_PROGRAM_TYPE      = 1136
Const CG_VERTEXSHADER_TYPE = 1141
Const CG_TYPE_START_ENUM   = 1024
Const CG_SAMPLER           = 1143
Const CG_SAMPLER1D         = 1065
Const CG_SAMPLER1DARRAY    = 1138
Const CG_SAMPLER1DSHADOW   = 1313
Const CG_SAMPLER2D         = 1066
Const CG_SAMPLER2DARRAY    = 1139
Const CG_SAMPLER2DSHADOW   = 1314
Const CG_SAMPLER3D         = 1067
Const CG_SAMPLERBUF        = 1144
Const CG_SAMPLERCUBE       = 1069
Const CG_SAMPLERCUBEARRAY  = 1140
Const CG_SAMPLERRECT       = 1068
Const CG_SAMPLERRECTSHADOW = 1315
Const CG_BOOL              = 1114
Const CG_BOOL1             = 1115
Const CG_BOOL2             = 1116
Const CG_BOOL3             = 1117
Const CG_BOOL4             = 1118
Const CG_BOOL1x1           = 1119
Const CG_BOOL1x2           = 1120
Const CG_BOOL1x3           = 1121
Const CG_BOOL1x4           = 1122
Const CG_BOOL2x1           = 1123
Const CG_BOOL2x2           = 1124
Const CG_BOOL2x3           = 1125
Const CG_BOOL2x4           = 1126
Const CG_BOOL3x1           = 1127
Const CG_BOOL3x2           = 1128
Const CG_BOOL3x3           = 1129
Const CG_BOOL3x4           = 1130
Const CG_BOOL4x1           = 1131
Const CG_BOOL4x2           = 1132
Const CG_BOOL4x3           = 1133
Const CG_BOOL4x4           = 1134
Const CG_CHAR              = 1166
Const CG_CHAR1             = 1167
Const CG_CHAR2             = 1168
Const CG_CHAR3             = 1169
Const CG_CHAR4             = 1170
Const CG_CHAR1x1           = 1171
Const CG_CHAR1x2           = 1172
Const CG_CHAR1x3           = 1173
Const CG_CHAR1x4           = 1174
Const CG_CHAR2x1           = 1175
Const CG_CHAR2x2           = 1176
Const CG_CHAR2x3           = 1177
Const CG_CHAR2x4           = 1178
Const CG_CHAR3x1           = 1179
Const CG_CHAR3x2           = 1180
Const CG_CHAR3x3           = 1181
Const CG_CHAR3x4           = 1182
Const CG_CHAR4x1           = 1183
Const CG_CHAR4x2           = 1184
Const CG_CHAR4x3           = 1185
Const CG_CHAR4x4           = 1186
Const CG_DOUBLE            = 1145
Const CG_DOUBLE1           = 1146
Const CG_DOUBLE2           = 1147
Const CG_DOUBLE3           = 1148
Const CG_DOUBLE4           = 1149
Const CG_DOUBLE1x1         = 1150
Const CG_DOUBLE1x2         = 1151
Const CG_DOUBLE1x3         = 1152
Const CG_DOUBLE1x4         = 1153
Const CG_DOUBLE2x1         = 1154
Const CG_DOUBLE2x2         = 1155
Const CG_DOUBLE2x3         = 1156
Const CG_DOUBLE2x4         = 1157
Const CG_DOUBLE3x1         = 1158
Const CG_DOUBLE3x2         = 1159
Const CG_DOUBLE3x3         = 1160
Const CG_DOUBLE3x4         = 1161
Const CG_DOUBLE4x1         = 1162
Const CG_DOUBLE4x2         = 1163
Const CG_DOUBLE4x3         = 1164
Const CG_DOUBLE4x4         = 1165
Const CG_FIXED             = 1070
Const CG_FIXED1            = 1092
Const CG_FIXED2            = 1071
Const CG_FIXED3            = 1072
Const CG_FIXED4            = 1073
Const CG_FIXED1x1          = 1074
Const CG_FIXED1x2          = 1075
Const CG_FIXED1x3          = 1076
Const CG_FIXED1x4          = 1077
Const CG_FIXED2x1          = 1078
Const CG_FIXED2x2          = 1079
Const CG_FIXED2x3          = 1080
Const CG_FIXED2x4          = 1081
Const CG_FIXED3x1          = 1082
Const CG_FIXED3x2          = 1083
Const CG_FIXED3x3          = 1084
Const CG_FIXED3x4          = 1085
Const CG_FIXED4x1          = 1086
Const CG_FIXED4x2          = 1087
Const CG_FIXED4x3          = 1088
Const CG_FIXED4x4          = 1089
Const CG_FLOAT             = 1045
Const CG_FLOAT1            = 1091
Const CG_FLOAT2            = 1046
Const CG_FLOAT3            = 1047
Const CG_FLOAT4            = 1048
Const CG_FLOAT1x1          = 1049
Const CG_FLOAT1x2          = 1050
Const CG_FLOAT1x3          = 1051
Const CG_FLOAT1x4          = 1052
Const CG_FLOAT2x1          = 1053
Const CG_FLOAT2x2          = 1054
Const CG_FLOAT2x3          = 1055
Const CG_FLOAT2x4          = 1056
Const CG_FLOAT3x1          = 1057
Const CG_FLOAT3x2          = 1058
Const CG_FLOAT3x3          = 1059
Const CG_FLOAT3x4          = 1060
Const CG_FLOAT4x1          = 1061
Const CG_FLOAT4x2          = 1062
Const CG_FLOAT4x3          = 1063
Const CG_FLOAT4x4          = 1064
Const CG_HALF              = 1025
Const CG_HALF1             = 1090
Const CG_HALF2             = 1026
Const CG_HALF3             = 1027
Const CG_HALF4             = 1028
Const CG_HALF1x1           = 1029
Const CG_HALF1x2           = 1030
Const CG_HALF1x3           = 1031
Const CG_HALF1x4           = 1032
Const CG_HALF2x1           = 1033
Const CG_HALF2x2           = 1034
Const CG_HALF2x3           = 1035
Const CG_HALF2x4           = 1036
Const CG_HALF3x1           = 1037
Const CG_HALF3x2           = 1038
Const CG_HALF3x3           = 1039
Const CG_HALF3x4           = 1040
Const CG_HALF4x1           = 1041
Const CG_HALF4x2           = 1042
Const CG_HALF4x3           = 1043
Const CG_HALF4x4           = 1044
Const CG_INT               = 1093
Const CG_INT1              = 1094
Const CG_INT2              = 1095
Const CG_INT3              = 1096
Const CG_INT4              = 1097
Const CG_INT1x1            = 1098
Const CG_INT1x2            = 1099
Const CG_INT1x3            = 1100
Const CG_INT1x4            = 1101
Const CG_INT2x1            = 1102
Const CG_INT2x2            = 1103
Const CG_INT2x3            = 1104
Const CG_INT2x4            = 1105
Const CG_INT3x1            = 1106
Const CG_INT3x2            = 1107
Const CG_INT3x3            = 1108
Const CG_INT3x4            = 1109
Const CG_INT4x1            = 1110
Const CG_INT4x2            = 1111
Const CG_INT4x3            = 1112
Const CG_INT4x4            = 1113
Const CG_LONG              = 1271
Const CG_LONG1             = 1272
Const CG_LONG2             = 1273
Const CG_LONG3             = 1274
Const CG_LONG4             = 1275
Const CG_LONG1x1           = 1276
Const CG_LONG1x2           = 1277
Const CG_LONG1x3           = 1278
Const CG_LONG1x4           = 1279
Const CG_LONG2x1           = 1280
Const CG_LONG2x2           = 1281
Const CG_LONG2x3           = 1282
Const CG_LONG2x4           = 1283
Const CG_LONG3x1           = 1284
Const CG_LONG3x2           = 1285
Const CG_LONG3x3           = 1286
Const CG_LONG3x4           = 1287
Const CG_LONG4x1           = 1288
Const CG_LONG4x2           = 1289
Const CG_LONG4x3           = 1290
Const CG_LONG4x4           = 1291
Const CG_SHORT             = 1208
Const CG_SHORT1            = 1209
Const CG_SHORT2            = 1210
Const CG_SHORT3            = 1211
Const CG_SHORT4            = 1212
Const CG_SHORT1x1          = 1213
Const CG_SHORT1x2          = 1214
Const CG_SHORT1x3          = 1215
Const CG_SHORT1x4          = 1216
Const CG_SHORT2x1          = 1217
Const CG_SHORT2x2          = 1218
Const CG_SHORT2x3          = 1219
Const CG_SHORT2x4          = 1220
Const CG_SHORT3x1          = 1221
Const CG_SHORT3x2          = 1222
Const CG_SHORT3x3          = 1223
Const CG_SHORT3x4          = 1224
Const CG_SHORT4x1          = 1225
Const CG_SHORT4x2          = 1226
Const CG_SHORT4x3          = 1227
Const CG_SHORT4x4          = 1228
Const CG_UCHAR             = 1187
Const CG_UCHAR1            = 1188
Const CG_UCHAR2            = 1189
Const CG_UCHAR3            = 1190
Const CG_UCHAR4            = 1191
Const CG_UCHAR1x1          = 1192
Const CG_UCHAR1x2          = 1193
Const CG_UCHAR1x3          = 1194
Const CG_UCHAR1x4          = 1195
Const CG_UCHAR2x1          = 1196
Const CG_UCHAR2x2          = 1197
Const CG_UCHAR2x3          = 1198
Const CG_UCHAR2x4          = 1199
Const CG_UCHAR3x1          = 1200
Const CG_UCHAR3x2          = 1201
Const CG_UCHAR3x3          = 1202
Const CG_UCHAR3x4          = 1203
Const CG_UCHAR4x1          = 1204
Const CG_UCHAR4x2          = 1205
Const CG_UCHAR4x3          = 1206
Const CG_UCHAR4x4          = 1207
Const CG_UINT              = 1250
Const CG_UINT1             = 1251
Const CG_UINT2             = 1252
Const CG_UINT3             = 1253
Const CG_UINT4             = 1254
Const CG_UINT1x1           = 1255
Const CG_UINT1x2           = 1256
Const CG_UINT1x3           = 1257
Const CG_UINT1x4           = 1258
Const CG_UINT2x1           = 1259
Const CG_UINT2x2           = 1260
Const CG_UINT2x3           = 1261
Const CG_UINT2x4           = 1262
Const CG_UINT3x1           = 1263
Const CG_UINT3x2           = 1264
Const CG_UINT3x3           = 1265
Const CG_UINT3x4           = 1266
Const CG_UINT4x1           = 1267
Const CG_UINT4x2           = 1268
Const CG_UINT4x3           = 1269
Const CG_UINT4x4           = 1270
Const CG_ULONG             = 1292
Const CG_ULONG1            = 1293
Const CG_ULONG2            = 1294
Const CG_ULONG3            = 1295
Const CG_ULONG4            = 1296
Const CG_ULONG1x1          = 1297
Const CG_ULONG1x2          = 1298
Const CG_ULONG1x3          = 1299
Const CG_ULONG1x4          = 1300
Const CG_ULONG2x1          = 1301
Const CG_ULONG2x2          = 1302
Const CG_ULONG2x3          = 1303
Const CG_ULONG2x4          = 1304
Const CG_ULONG3x1          = 1305
Const CG_ULONG3x2          = 1306
Const CG_ULONG3x3          = 1307
Const CG_ULONG3x4          = 1308
Const CG_ULONG4x1          = 1309
Const CG_ULONG4x2          = 1310
Const CG_ULONG4x3          = 1311
Const CG_ULONG4x4          = 1312
Const CG_USHORT            = 1229
Const CG_USHORT1           = 1230
Const CG_USHORT2           = 1231
Const CG_USHORT3           = 1232
Const CG_USHORT4           = 1233
Const CG_USHORT1x1         = 1234
Const CG_USHORT1x2         = 1235
Const CG_USHORT1x3         = 1236
Const CG_USHORT1x4         = 1237
Const CG_USHORT2x1         = 1238
Const CG_USHORT2x2         = 1239
Const CG_USHORT2x3         = 1240
Const CG_USHORT2x4         = 1241
Const CG_USHORT3x1         = 1242
Const CG_USHORT3x2         = 1243
Const CG_USHORT3x3         = 1244
Const CG_USHORT3x4         = 1245
Const CG_USHORT4x1         = 1246
Const CG_USHORT4x2         = 1247
Const CG_USHORT4x3         = 1248
Const CG_USHORT4x4         = 1249


' CGresource
Const CG_TEXUNIT0              = 2048
Const CG_TEXUNIT1              = 2049
Const CG_TEXUNIT2              = 2050
Const CG_TEXUNIT3              = 2051
Const CG_TEXUNIT4              = 2052
Const CG_TEXUNIT5              = 2053
Const CG_TEXUNIT6              = 2054
Const CG_TEXUNIT7              = 2055
Const CG_TEXUNIT8              = 2056
Const CG_TEXUNIT9              = 2057
Const CG_TEXUNIT10             = 2058
Const CG_TEXUNIT11             = 2059
Const CG_TEXUNIT12             = 2060
Const CG_TEXUNIT13             = 2061
Const CG_TEXUNIT14             = 2062
Const CG_TEXUNIT15             = 2063
Const CG_TEXUNIT16             = 4624
Const CG_TEXUNIT17             = 4625
Const CG_TEXUNIT18             = 4626
Const CG_TEXUNIT19             = 4627
Const CG_TEXUNIT20             = 4628
Const CG_TEXUNIT21             = 4629
Const CG_TEXUNIT22             = 4630
Const CG_TEXUNIT23             = 4631
Const CG_TEXUNIT24             = 4632
Const CG_TEXUNIT25             = 4633
Const CG_TEXUNIT26             = 4634
Const CG_TEXUNIT27             = 4635
Const CG_TEXUNIT28             = 4636
Const CG_TEXUNIT29             = 4637
Const CG_TEXUNIT30             = 4638
Const CG_TEXUNIT31             = 4639
Const CG_BUFFER0               = 2064
Const CG_BUFFER1               = 2065
Const CG_BUFFER2               = 2066
Const CG_BUFFER3               = 2067
Const CG_BUFFER4               = 2068
Const CG_BUFFER5               = 2069
Const CG_BUFFER6               = 2070
Const CG_BUFFER7               = 2071
Const CG_BUFFER8               = 2072
Const CG_BUFFER9               = 2073
Const CG_BUFFER10              = 2074
Const CG_BUFFER11              = 2075
Const CG_ATTR0                 = 2113
Const CG_ATTR1                 = 2114
Const CG_ATTR2                 = 2115
Const CG_ATTR3                 = 2116
Const CG_ATTR4                 = 2117
Const CG_ATTR5                 = 2118
Const CG_ATTR6                 = 2119
Const CG_ATTR7                 = 2120
Const CG_ATTR8                 = 2121
Const CG_ATTR9                 = 2122
Const CG_ATTR10                = 2123
Const CG_ATTR11                = 2124
Const CG_ATTR12                = 2125
Const CG_ATTR13                = 2126
Const CG_ATTR14                = 2127
Const CG_ATTR15                = 2128
Const CG_C                     = 2178
Const CG_TEX0                  = 2179
Const CG_TEX1                  = 2180
Const CG_TEX2                  = 2181
Const CG_TEX3                  = 2192
Const CG_TEX4                  = 2193
Const CG_TEX5                  = 2194
Const CG_TEX6                  = 2195
Const CG_TEX7                  = 2196
Const CG_HPOS                  = 2243
Const CG_COL0                  = 2245
Const CG_COL1                  = 2246
Const CG_COL2                  = 2247
Const CG_COL3                  = 2248
Const CG_PSIZ                  = 2309
Const CG_CLP0                  = 2310
Const CG_CLP1                  = 2311
Const CG_CLP2                  = 2312
Const CG_CLP3                  = 2313
Const CG_CLP4                  = 2314
Const CG_CLP5                  = 2315
Const CG_WPOS                  = 2373
Const CG_POINTCOORD            = 2374
Const CG_POSITION0             = 2437
Const CG_POSITION1             = 2438
Const CG_POSITION2             = 2439
Const CG_POSITION3             = 2440
Const CG_POSITION4             = 2441
Const CG_POSITION5             = 2442
Const CG_POSITION6             = 2443
Const CG_POSITION7             = 2444
Const CG_POSITION8             = 2445
Const CG_POSITION9             = 2446
Const CG_POSITION10            = 2447
Const CG_POSITION11            = 2448
Const CG_POSITION12            = 2449
Const CG_POSITION13            = 2450
Const CG_POSITION14            = 2451
Const CG_POSITION15            = 2452
Const CG_DIFFUSE0              = 2501
Const CG_TANGENT0              = 2565
Const CG_TANGENT1              = 2566
Const CG_TANGENT2              = 2567
Const CG_TANGENT3              = 2568
Const CG_TANGENT4              = 2569
Const CG_TANGENT5              = 2570
Const CG_TANGENT6              = 2571
Const CG_TANGENT7              = 2572
Const CG_TANGENT8              = 2573
Const CG_TANGENT9              = 2574
Const CG_TANGENT10             = 2575
Const CG_TANGENT11             = 2576
Const CG_TANGENT12             = 2577
Const CG_TANGENT13             = 2578
Const CG_TANGENT14             = 2579
Const CG_TANGENT15             = 2580
Const CG_SPECULAR0             = 2629
Const CG_BLENDINDICES0         = 2693
Const CG_BLENDINDICES1         = 2694
Const CG_BLENDINDICES2         = 2695
Const CG_BLENDINDICES3         = 2696
Const CG_BLENDINDICES4         = 2697
Const CG_BLENDINDICES5         = 2698
Const CG_BLENDINDICES6         = 2699
Const CG_BLENDINDICES7         = 2700
Const CG_BLENDINDICES8         = 2701
Const CG_BLENDINDICES9         = 2702
Const CG_BLENDINDICES10        = 2703
Const CG_BLENDINDICES11        = 2704
Const CG_BLENDINDICES12        = 2705
Const CG_BLENDINDICES13        = 2706
Const CG_BLENDINDICES14        = 2707
Const CG_BLENDINDICES15        = 2708
Const CG_COLOR0                = 2757
Const CG_COLOR1                = 2758
Const CG_COLOR2                = 2759
Const CG_COLOR3                = 2760
Const CG_COLOR4                = 2761
Const CG_COLOR5                = 2762
Const CG_COLOR6                = 2763
Const CG_COLOR7                = 2764
Const CG_COLOR8                = 2765
Const CG_COLOR9                = 2766
Const CG_COLOR10               = 2767
Const CG_COLOR11               = 2768
Const CG_COLOR12               = 2769
Const CG_COLOR13               = 2770
Const CG_COLOR14               = 2771
Const CG_COLOR15               = 2772
Const CG_PSIZE0                = 2821
Const CG_PSIZE1                = 2822
Const CG_PSIZE2                = 2823
Const CG_PSIZE3                = 2824
Const CG_PSIZE4                = 2825
Const CG_PSIZE5                = 2826
Const CG_PSIZE6                = 2827
Const CG_PSIZE7                = 2828
Const CG_PSIZE8                = 2829
Const CG_PSIZE9                = 2830
Const CG_PSIZE10               = 2831
Const CG_PSIZE11               = 2832
Const CG_PSIZE12               = 2833
Const CG_PSIZE13               = 2834
Const CG_PSIZE14               = 2835
Const CG_PSIZE15               = 2836
Const CG_BINORMAL0             = 2885
Const CG_BINORMAL1             = 2886
Const CG_BINORMAL2             = 2887
Const CG_BINORMAL3             = 2888
Const CG_BINORMAL4             = 2889
Const CG_BINORMAL5             = 2890
Const CG_BINORMAL6             = 2891
Const CG_BINORMAL7             = 2892
Const CG_BINORMAL8             = 2893
Const CG_BINORMAL9             = 2894
Const CG_BINORMAL10            = 2895
Const CG_BINORMAL11            = 2896
Const CG_BINORMAL12            = 2897
Const CG_BINORMAL13            = 2898
Const CG_BINORMAL14            = 2899
Const CG_BINORMAL15            = 2900
Const CG_FOG0                  = 2917
Const CG_FOG1                  = 2918
Const CG_FOG2                  = 2919
Const CG_FOG3                  = 2920
Const CG_FOG4                  = 2921
Const CG_FOG5                  = 2922
Const CG_FOG6                  = 2923
Const CG_FOG7                  = 2924
Const CG_FOG8                  = 2925
Const CG_FOG9                  = 2926
Const CG_FOG10                 = 2927
Const CG_FOG11                 = 2928
Const CG_FOG12                 = 2929
Const CG_FOG13                 = 2930
Const CG_FOG14                 = 2931
Const CG_FOG15                 = 2932
Const CG_DEPTH0                = 2933
Const CG_DEPTH1                = 2934
Const CG_DEPTH2                = 2935
Const CG_DEPTH3                = 2936
Const CG_DEPTH4                = 2937
Const CG_DEPTH5                = 2938
Const CG_DEPTH6                = 2939
Const CG_DEPTH7                = 2940
Const CG_DEPTH8                = 2941
Const CG_DEPTH9                = 2942
Const CG_DEPTH10               = 2943
Const CG_DEPTH11               = 2944
Const CG_DEPTH12               = 2945
Const CG_DEPTH13               = 2946
Const CG_DEPTH14               = 2947
Const CG_DEPTH15               = 2948
Const CG_SAMPLE0               = 2949
Const CG_SAMPLE1               = 2950
Const CG_SAMPLE2               = 2951
Const CG_SAMPLE3               = 2952
Const CG_SAMPLE4               = 2953
Const CG_SAMPLE5               = 2954
Const CG_SAMPLE6               = 2955
Const CG_SAMPLE7               = 2956
Const CG_SAMPLE8               = 2957
Const CG_SAMPLE9               = 2958
Const CG_SAMPLE10              = 2959
Const CG_SAMPLE11              = 2960
Const CG_SAMPLE12              = 2961
Const CG_SAMPLE13              = 2962
Const CG_SAMPLE14              = 2963
Const CG_SAMPLE15              = 2964
Const CG_BLENDWEIGHT0          = 3028
Const CG_BLENDWEIGHT1          = 3029
Const CG_BLENDWEIGHT2          = 3030
Const CG_BLENDWEIGHT3          = 3031
Const CG_BLENDWEIGHT4          = 3032
Const CG_BLENDWEIGHT5          = 3033
Const CG_BLENDWEIGHT6          = 3034
Const CG_BLENDWEIGHT7          = 3035
Const CG_BLENDWEIGHT8          = 3036
Const CG_BLENDWEIGHT9          = 3037
Const CG_BLENDWEIGHT10         = 3038
Const CG_BLENDWEIGHT11         = 3039
Const CG_BLENDWEIGHT12         = 3040
Const CG_BLENDWEIGHT13         = 3041
Const CG_BLENDWEIGHT14         = 3042
Const CG_BLENDWEIGHT15         = 3043
Const CG_NORMAL0               = 3092
Const CG_NORMAL1               = 3093
Const CG_NORMAL2               = 3094
Const CG_NORMAL3               = 3095
Const CG_NORMAL4               = 3096
Const CG_NORMAL5               = 3097
Const CG_NORMAL6               = 3098
Const CG_NORMAL7               = 3099
Const CG_NORMAL8               = 3100
Const CG_NORMAL9               = 3101
Const CG_NORMAL10              = 3102
Const CG_NORMAL11              = 3103
Const CG_NORMAL12              = 3104
Const CG_NORMAL13              = 3105
Const CG_NORMAL14              = 3106
Const CG_NORMAL15              = 3107
Const CG_FOGCOORD              = 3156
Const CG_TEXCOORD0             = 3220
Const CG_TEXCOORD1             = 3221
Const CG_TEXCOORD2             = 3222
Const CG_TEXCOORD3             = 3223
Const CG_TEXCOORD4             = 3224
Const CG_TEXCOORD5             = 3225
Const CG_TEXCOORD6             = 3226
Const CG_TEXCOORD7             = 3227
Const CG_TEXCOORD8             = 3228
Const CG_TEXCOORD9             = 3229
Const CG_TEXCOORD10            = 3230
Const CG_TEXCOORD11            = 3231
Const CG_TEXCOORD12            = 3232
Const CG_TEXCOORD13            = 3233
Const CG_TEXCOORD14            = 3234
Const CG_TEXCOORD15            = 3235
Const CG_COMBINER_CONST0       = 3284
Const CG_COMBINER_CONST1       = 3285
Const CG_COMBINER_STAGE_CONST0 = 3286
Const CG_COMBINER_STAGE_CONST1 = 3287
Const CG_OFFSET_TEXTURE_MATRIX = 3288
Const CG_OFFSET_TEXTURE_SCALE  = 3289
Const CG_OFFSET_TEXTURE_BIAS   = 3290
Const CG_CONST_EYE             = 3291
Const CG_COVERAGE              = 3292
Const CG_TESSFACTOR            = 3255
Const CG_GLSL_UNIFORM          = 3300
Const CG_GLSL_ATTRIB           = 3301
Const CG_ENV                   = 3302
Const CG_HLSL_UNIFORM          = 3559
Const CG_HLSL_VARYING          = 3560
Const CG_SAMPLER_RES           = 3561
Const CG_LASTCOL0              = 4400
Const CG_LASTCOL1              = 4401
Const CG_LASTCOL2              = 4402
Const CG_LASTCOL3              = 4403
Const CG_LASTCOL4              = 4404
Const CG_LASTCOL5              = 4405
Const CG_LASTCOL6              = 4406
Const CG_LASTCOL7              = 4407
Const CG_FACE                  = 4410
Const CG_PRIMITIVEID           = 4411
Const CG_INSTANCEID            = 4412
Const CG_SAMPLEID              = 4413
Const CG_VERTEXID              = 4414
Const CG_LAYER                 = 4415
Const CG_SAMPLEMASK            = 4416
Const CG_CONTROLPOINTID        = 4417
Const CG_EDGETESS              = 4418
Const CG_INNERTESS             = 4419
Const CG_UNDEFINED             = 3256


' CGprofile
Const CG_PROFILE_UNKNOWN = 6145
Const CG_PROFILE_VP20    = 6146
Const CG_PROFILE_FP20    = 6147
Const CG_PROFILE_VP30    = 6148
Const CG_PROFILE_FP30    = 6149
Const CG_PROFILE_ARBVP1  = 6150
Const CG_PROFILE_FP40    = 6151
Const CG_PROFILE_ARBFP1  = 7000
Const CG_PROFILE_VP40    = 7001
Const CG_PROFILE_GLSLV   = 7007
Const CG_PROFILE_GLSLF   = 7008
Const CG_PROFILE_GLSLG   = 7016
Const CG_PROFILE_GLSLC   = 7009
Const CG_PROFILE_GPU_FP  = 7010
Const CG_PROFILE_GPU_VP  = 7011
Const CG_PROFILE_GPU_GP  = 7012
Const CG_PROFILE_GP4FP   = 7010
Const CG_PROFILE_GP4VP   = 7011
Const CG_PROFILE_GP4GP   = 7012
Const CG_PROFILE_GP5FP   = 7017
Const CG_PROFILE_GP5VP   = 7018
Const CG_PROFILE_GP5GP   = 7019
Const CG_PROFILE_GP5TCP  = 7020
Const CG_PROFILE_GP5TEP  = 7021
Const CG_PROFILE_VS_1_1  = 6153
Const CG_PROFILE_VS_2_0  = 6154
Const CG_PROFILE_VS_2_X  = 6155
Const CG_PROFILE_VS_2_SW = 6156
Const CG_PROFILE_VS_3_0  = 6157
Const CG_PROFILE_HLSLV   = 6158
Const CG_PROFILE_PS_1_1  = 6159
Const CG_PROFILE_PS_1_2  = 6160
Const CG_PROFILE_PS_1_3  = 6161
Const CG_PROFILE_PS_2_0  = 6162
Const CG_PROFILE_PS_2_X  = 6163
Const CG_PROFILE_PS_2_SW = 6164
Const CG_PROFILE_PS_3_0  = 6165
Const CG_PROFILE_HLSLF   = 6166
Const CG_PROFILE_VS_4_0  = 6167
Const CG_PROFILE_PS_4_0  = 6168
Const CG_PROFILE_GS_4_0  = 6169
Const CG_PROFILE_VS_5_0  = 6170
Const CG_PROFILE_PS_5_0  = 6171
Const CG_PROFILE_GS_5_0  = 6172
Const CG_PROFILE_HS_5_0  = 6173
Const CG_PROFILE_DS_5_0  = 6174
Const CG_PROFILE_GENERIC = 7002


' CGerror
Const CG_NO_ERROR                               = 0
Const CG_COMPILER_ERROR                         = 1
Const CG_INVALID_PARAMETER_ERROR                = 2
Const CG_INVALID_PROFILE_ERROR                  = 3
Const CG_PROGRAM_LOAD_ERROR                     = 4
Const CG_PROGRAM_BIND_ERROR                     = 5
Const CG_PROGRAM_NOT_LOADED_ERROR               = 6
Const CG_UNSUPPORTED_GL_EXTENSION_ERROR         = 7
Const CG_INVALID_VALUE_TYPE_ERROR               = 8
Const CG_NOT_MATRIX_PARAM_ERROR                 = 9
Const CG_INVALID_ENUMERANT_ERROR                = 10
Const CG_NOT_4x4_MATRIX_ERROR                   = 11
Const CG_FILE_READ_ERROR                        = 12
Const CG_FILE_WRITE_ERROR                       = 13
Const CG_NVPARSE_ERROR                          = 14
Const CG_MEMORY_ALLOC_ERROR                     = 15
Const CG_INVALID_CONTEXT_HANDLE_ERROR           = 16
Const CG_INVALID_PROGRAM_HANDLE_ERROR           = 17
Const CG_INVALID_PARAM_HANDLE_ERROR             = 18
Const CG_UNKNOWN_PROFILE_ERROR                  = 19
Const CG_VAR_ARG_ERROR                          = 20
Const CG_INVALID_DIMENSION_ERROR                = 21
Const CG_ARRAY_PARAM_ERROR                      = 22
Const CG_OUT_OF_ARRAY_BOUNDS_ERROR              = 23
Const CG_CONFLICTING_TYPES_ERROR                = 24
Const CG_CONFLICTING_PARAMETER_TYPES_ERROR      = 25
Const CG_PARAMETER_IS_NOT_SHARED_ERROR          = 26
Const CG_INVALID_PARAMETER_VARIABILITY_ERROR    = 27
Const CG_CANNOT_DESTROY_PARAMETER_ERROR         = 28
Const CG_NOT_ROOT_PARAMETER_ERROR               = 29
Const CG_PARAMETERS_DO_NOT_MATCH_ERROR          = 30
Const CG_IS_NOT_PROGRAM_PARAMETER_ERROR         = 31
Const CG_INVALID_PARAMETER_TYPE_ERROR           = 32
Const CG_PARAMETER_IS_NOT_RESIZABLE_ARRAY_ERROR = 33
Const CG_INVALID_SIZE_ERROR                     = 34
Const CG_BIND_CREATES_CYCLE_ERROR               = 35
Const CG_ARRAY_TYPES_DO_NOT_MATCH_ERROR         = 36
Const CG_ARRAY_DIMENSIONS_DO_NOT_MATCH_ERROR    = 37
Const CG_ARRAY_HAS_WRONG_DIMENSION_ERROR        = 38
Const CG_TYPE_IS_NOT_DEFINED_IN_PROGRAM_ERROR   = 39
Const CG_INVALID_EFFECT_HANDLE_ERROR            = 40
Const CG_INVALID_STATE_HANDLE_ERROR             = 41
Const CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR  = 42
Const CG_INVALID_PASS_HANDLE_ERROR              = 43
Const CG_INVALID_ANNOTATION_HANDLE_ERROR        = 44
Const CG_INVALID_TECHNIQUE_HANDLE_ERROR         = 45
Const CG_INVALID_PARAMETER_HANDLE_ERROR         = 46
Const CG_STATE_ASSIGNMENT_TYPE_MISMATCH_ERROR   = 47
Const CG_INVALID_FUNCTION_HANDLE_ERROR          = 48
Const CG_INVALID_TECHNIQUE_ERROR                = 49
Const CG_INVALID_POINTER_ERROR                  = 50
Const CG_NOT_ENOUGH_DATA_ERROR                  = 51
Const CG_NON_NUMERIC_PARAMETER_ERROR            = 52
Const CG_ARRAY_SIZE_MISMATCH_ERROR              = 53
Const CG_CANNOT_SET_NON_UNIFORM_PARAMETER_ERROR = 54
Const CG_DUPLICATE_NAME_ERROR                   = 55
Const CG_INVALID_OBJ_HANDLE_ERROR               = 56
Const CG_INVALID_BUFFER_HANDLE_ERROR            = 57
Const CG_BUFFER_INDEX_OUT_OF_RANGE_ERROR        = 58
Const CG_BUFFER_ALREADY_MAPPED_ERROR            = 59
Const CG_BUFFER_UPDATE_NOT_ALLOWED_ERROR        = 60
Const CG_GLSLG_UNCOMBINED_LOAD_ERROR            = 61


' CGenum
Const CG_UNKNOWN                            = 4096
Const CG_IN                                 = 4097
Const CG_OUT                                = 4098
Const CG_INOUT                              = 4099
Const CG_MIXED                              = 4100
Const CG_VARYING                            = 4101
Const CG_UNIFORM                            = 4102
Const CG_CONSTANT                           = 4103
Const CG_PROGRAM_SOURCE                     = 4104
Const CG_PROGRAM_ENTRY                      = 4105
Const CG_COMPILED_PROGRAM                   = 4106
Const CG_PROGRAM_PROFILE                    = 4107
Const CG_GLOBAL                             = 4108
Const CG_PROGRAM                            = 4109
Const CG_DEFAULT                            = 4110
Const CG_ERROR                              = 4111
Const CG_SOURCE                             = 4112
Const CG_OBJECT                             = 4113
Const CG_COMPILE_MANUAL                     = 4114
Const CG_COMPILE_IMMEDIATE                  = 4115
Const CG_COMPILE_LAZY                       = 4116
Const CG_CURRENT                            = 4117
Const CG_LITERAL                            = 4118
Const CG_VERSION                            = 4119
Const CG_ROW_MAJOR                          = 4120
Const CG_COLUMN_MAJOR                       = 4121
Const CG_FRAGMENT                           = 4122
Const CG_VERTEX                             = 4123
Const CG_POINT                              = 4124
Const CG_LINE                               = 4125
Const CG_LINE_ADJ                           = 4126
Const CG_TRIANGLE                           = 4127
Const CG_TRIANGLE_ADJ                       = 4128
Const CG_POINT_OUT                          = 4129
Const CG_LINE_OUT                           = 4130
Const CG_TRIANGLE_OUT                       = 4131
Const CG_IMMEDIATE_PARAMETER_SETTING        = 4132
Const CG_DEFERRED_PARAMETER_SETTING         = 4133
Const CG_NO_LOCKS_POLICY                    = 4134
Const CG_THREAD_SAFE_POLICY                 = 4135
Const CG_FORCE_UPPER_CASE_POLICY            = 4136
Const CG_UNCHANGED_CASE_POLICY              = 4137
Const CG_IS_OPENGL_PROFILE                  = 4138
Const CG_IS_DIRECT3D_PROFILE                = 4139
Const CG_IS_DIRECT3D_8_PROFILE              = 4140
Const CG_IS_DIRECT3D_9_PROFILE              = 4141
Const CG_IS_DIRECT3D_10_PROFILE             = 4142
Const CG_IS_VERTEX_PROFILE                  = 4143
Const CG_IS_FRAGMENT_PROFILE                = 4144
Const CG_IS_GEOMETRY_PROFILE                = 4145
Const CG_IS_TRANSLATION_PROFILE             = 4146
Const CG_IS_HLSL_PROFILE                    = 4147
Const CG_IS_GLSL_PROFILE                    = 4148
Const CG_IS_TESSELLATION_CONTROL_PROFILE    = 4149
Const CG_IS_TESSELLATION_EVALUATION_PROFILE = 4150
Const CG_PATCH                              = 4152
Const CG_IS_DIRECT3D_11_PROFILE             = 4153

' CGparameterclass
Const CG_PARAMETERCLASS_UNKNOWN = 0
Const CG_PARAMETERCLASS_SCALAR  = 1
Const CG_PARAMETERCLASS_VECTOR  = 2
Const CG_PARAMETERCLASS_MATRIX  = 3
Const CG_PARAMETERCLASS_STRUCT  = 4
Const CG_PARAMETERCLASS_ARRAY   = 5
Const CG_PARAMETERCLASS_SAMPLER = 6
Const CG_PARAMETERCLASS_OBJECT  = 7

' CGdomain
Const CG_UNKNOWN_DOMAIN                 = 0
Const CG_FIRST_DOMAIN                   = 1
Const CG_VERTEX_DOMAIN                  = 1
Const CG_FRAGMENT_DOMAIN                = 2
Const CG_GEOMETRY_DOMAIN                = 3
Const CG_TESSELLATION_CONTROL_DOMAIN    = 4
Const CG_TESSELLATION_EVALUATION_DOMAIN = 5

' CGbufferaccess
Const CG_MAP_READ               = 0
Const CG_MAP_WRITE              = 1
Const CG_MAP_READ_WRITE         = 2
Const CG_MAP_WRITE_DISCARD      = 3
Const CG_MAP_WRITE_NO_OVERWRITE = 4

' CGbufferusage
Const CG_BUFFER_USAGE_STREAM_DRAW  = 0
Const CG_BUFFER_USAGE_STREAM_READ  = 1
Const CG_BUFFER_USAGE_STREAM_COPY  = 2
Const CG_BUFFER_USAGE_STATIC_DRAW  = 3
Const CG_BUFFER_USAGE_STATIC_READ  = 4
Const CG_BUFFER_USAGE_STATIC_COPY  = 5
Const CG_BUFFER_USAGE_DYNAMIC_DRAW = 6
Const CG_BUFFER_USAGE_DYNAMIC_READ = 7
Const CG_BUFFER_USAGE_DYNAMIC_COPY = 8

' Function CGstatecallback(sa)
' Function CGerrorCallbackFunc()
' Function CGerrorHandlerFunc(context,error,data:Byte Ptr)
' Function CGIncludeCallbackFunc(context, inc:Byte Ptr)

Extern
	Function cgSetLockingPolicy(lockingPolicy)
	Function cgGetLockingPolicy()
	Function cgSetSemanticCasePolicy(casePolicy)
	Function cgGetSemanticCasePolicy()
	Function cgCreateContext()
	Function cgSetContextBehavior(context, behavior)
	Function cgGetContextBehavior(context)
	Function cgGetBehaviorString:Byte Ptr(behavior)
	Function cgGetBehavior(behavior_string:Byte Ptr)
	Function cgDestroyContext(context)
	Function cgIsContext(context)
	'CG_API const char * CGENTRY cgGetLastListing(context)
	'Function cgSetLastListing(CGhandle handle, const char *listing)
	'Function cgSetAutoCompile(context, CGenum autoCompileMode)
	'Function cgGetAutoCompile(context)
	'Function cgSetParameterSettingMode(context, CGenum parameterSettingMode)
	'Function cgGetParameterSettingMode(context)
	'Function cgSetCompilerIncludeString(context, const char *name, const char *source)
	'Function cgSetCompilerIncludeFile(context, const char *name, const char *filename)
	'Function cgSetCompilerIncludeCallback(context, CGIncludeCallbackFunc func)
	'CG_API CGIncludeCallbackFunc CGENTRY cgGetCompilerIncludeCallback(context)
	'Function cgCreateProgram(context, CGenum program_type, const char *program, CGprofile profile, const char *entry, const char **args)
	'Function cgCreateProgramFromFile(context, CGenum program_type, const char *program_file, CGprofile profile, const char *entry, const char **args)
	'Function cgCopyProgram(program)
	'Function cgDestroyProgram(program)
	'Function cgGetFirstProgram(context)
	'Function cgGetNextProgram(CGprogram current)
	'CG_API CGcontext CGENTRY cgGetProgramContext(program)
	'Function cgIsProgram(program)
	'Function cgCompileProgram(program)
	'Function cgIsProgramCompiled(program)
	'CG_API const char * CGENTRY cgGetProgramString(program, CGenum pname)
	'CG_API CGprofile CGENTRY cgGetProgramProfile(program)
	'CG_API char const * const * CGENTRY cgGetProgramOptions(program)
	'Function cgSetProgramProfile(program, CGprofile profile)
	'Function cgGetProgramInput(program)
	'Function cgGetProgramOutput(program)
	'Function cgSetPassProgramParameters(program)
	'Function cgUpdateProgramParameters(program)
	'Function cgUpdatePassParameters(CGpass pass)
	'Function cgCreateParameter(context, CGtype type)
	'Function cgCreateParameterArray(context, CGtype type, int length)
	'Function cgCreateParameterMultiDimArray(context, CGtype type, int dim, const int *lengths)
	'Function cgDestroyParameter(param)
	'Function cgConnectParameter(CGparameter from, CGparameter to)
	'Function cgDisconnectParameter(param)
	'Function cgGetConnectedParameter(param)
	'Function cgGetNumConnectedToParameters(param)
	'Function cgGetConnectedToParameter(param, int index)
	'Function cgGetNamedParameter(program, const char *name)
	'Function cgGetNamedProgramParameter(program, CGenum name_space, const char *name)
	'Function cgGetFirstParameter(program, CGenum name_space)
	'Function cgGetNextParameter(CGparameter current)
	'Function cgGetFirstLeafParameter(program, CGenum name_space)
	'Function cgGetNextLeafParameter(CGparameter current)
	'Function cgGetFirstStructParameter(param)
	'Function cgGetNamedStructParameter(param, const char *name)
	'Function cgGetFirstDependentParameter(param)
	'Function cgGetArrayParameter(CGparameter aparam, int index)
	'Function cgGetArrayDimension(param)
	'CG_API CGtype CGENTRY cgGetArrayType(param)
	'Function cgGetArraySize(param, int dimension)
	'Function cgGetArrayTotalSize(param)
	'Function cgSetArraySize(param, int size)
	'Function cgSetMultiDimArraySize(param, const int *sizes)
	'Function cgGetParameterProgram(param)
	'CG_API CGcontext CGENTRY cgGetParameterContext(param)
	'Function cgIsParameter(param)
	'CG_API const char * CGENTRY cgGetParameterName(param)
	'CG_API CGtype CGENTRY cgGetParameterType(param)
	'CG_API CGtype CGENTRY cgGetParameterBaseType(param)
	'CG_API CGparameterclass CGENTRY cgGetParameterClass(param)
	'Function cgGetParameterRows(param)
	'Function cgGetParameterColumns(param)
	'CG_API CGtype CGENTRY cgGetParameterNamedType(param)
	'CG_API const char * CGENTRY cgGetParameterSemantic(param)
	'CG_API CGresource CGENTRY cgGetParameterResource(param)
	'CG_API CGresource CGENTRY cgGetParameterBaseResource(param)
	'CG_API unsigned long CGENTRY cgGetParameterResourceIndex(param)
	'Function cgGetParameterVariability(param)
	'Function cgGetParameterDirection(param)
	'Function cgIsParameterReferenced(param)
	'Function cgIsParameterUsed(param, CGhandle handle)
	'CG_API const double * CGENTRY cgGetParameterValues(param, CGenum value_type, int *nvalues)
	'Function cgSetParameterValuedr(param, int nelements, const double *vals)
	'Function cgSetParameterValuedc(param, int nelements, const double *vals)
	'Function cgSetParameterValuefr(param, int nelements, const float *vals)
	'Function cgSetParameterValuefc(param, int nelements, const float *vals)
	'Function cgSetParameterValueir(param, int nelements, const int *vals)
	'Function cgSetParameterValueic(param, int nelements, const int *vals)
	'Function cgGetParameterValuedr(param, int nelements, double *vals)
	'Function cgGetParameterValuedc(param, int nelements, double *vals)
	'Function cgGetParameterValuefr(param, int nelements, float *vals)
	'Function cgGetParameterValuefc(param, int nelements, float *vals)
	'Function cgGetParameterValueir(param, int nelements, int *vals)
	'Function cgGetParameterValueic(param, int nelements, int *vals)
	'Function cgGetParameterDefaultValuedr(param, int nelements, double *vals)
	'Function cgGetParameterDefaultValuedc(param, int nelements, double *vals)
	'Function cgGetParameterDefaultValuefr(param, int nelements, float *vals)
	'Function cgGetParameterDefaultValuefc(param, int nelements, float *vals)
	'Function cgGetParameterDefaultValueir(param, int nelements, int *vals)
	'Function cgGetParameterDefaultValueic(param, int nelements, int *vals)
	'CG_API const char * CGENTRY cgGetStringParameterValue(param)
	'Function cgSetStringParameterValue(param, const char *str)
	'Function cgGetParameterOrdinalNumber(param)
	'Function cgIsParameterGlobal(param)
	'Function cgGetParameterIndex(param)
	'Function cgSetParameterVariability(param, CGenum vary)
	'Function cgSetParameterSemantic(param, const char *semantic)
	'Function cgSetParameter1f(param, float x)
	'Function cgSetParameter2f(param, float x, float y)
	'Function cgSetParameter3f(param, float x, float y, float z)
	'Function cgSetParameter4f(param, float x, float y, float z, float w)
	'Function cgSetParameter1d(param, double x)
	'Function cgSetParameter2d(param, double x, double y)
	'Function cgSetParameter3d(param, double x, double y, double z)
	'Function cgSetParameter4d(param, double x, double y, double z, double w)
	'Function cgSetParameter1i(param, int x)
	'Function cgSetParameter2i(param, int x, int y)
	'Function cgSetParameter3i(param, int x, int y, int z)
	'Function cgSetParameter4i(param, int x, int y, int z, int w)
	'Function cgSetParameter1iv(param, const int *v)
	'Function cgSetParameter2iv(param, const int *v)
	'Function cgSetParameter3iv(param, const int *v)
	'Function cgSetParameter4iv(param, const int *v)
	'Function cgSetParameter1fv(param, const float *v)
	'Function cgSetParameter2fv(param, const float *v)
	'Function cgSetParameter3fv(param, const float *v)
	'Function cgSetParameter4fv(param, const float *v)
	'Function cgSetParameter1dv(param, const double *v)
	'Function cgSetParameter2dv(param, const double *v)
	'Function cgSetParameter3dv(param, const double *v)
	'Function cgSetParameter4dv(param, const double *v)
	'Function cgSetMatrixParameterir(param, const int *matrix)
	'Function cgSetMatrixParameterdr(param, const double *matrix)
	'Function cgSetMatrixParameterfr(param, const float *matrix)
	'Function cgSetMatrixParameteric(param, const int *matrix)
	'Function cgSetMatrixParameterdc(param, const double *matrix)
	'Function cgSetMatrixParameterfc(param, const float *matrix)
	'Function cgGetMatrixParameterir(param, int *matrix)
	'Function cgGetMatrixParameterdr(param, double *matrix)
	'Function cgGetMatrixParameterfr(param, float *matrix)
	'Function cgGetMatrixParameteric(param, int *matrix)
	'Function cgGetMatrixParameterdc(param, double *matrix)
	'Function cgGetMatrixParameterfc(param, float *matrix)
	'Function cgGetMatrixParameterOrder(param)
	'Function cgGetNamedSubParameter(param, const char *name)
	'CG_API const char * CGENTRY cgGetTypeString(CGtype type)
	'CG_API CGtype CGENTRY cgGetType(const char *type_string)
	'CG_API CGtype CGENTRY cgGetNamedUserType(CGhandle handle, const char *name)
	'Function cgGetNumUserTypes(CGhandle handle)
	'CG_API CGtype CGENTRY cgGetUserType(CGhandle handle, int index)
	'Function cgGetNumParentTypes(CGtype type)
	'CG_API CGtype CGENTRY cgGetParentType(CGtype type, int index)
	'Function cgIsParentType(CGtype parent, CGtype child)
	'Function cgIsInterfaceType(CGtype type)
	'CG_API const char * CGENTRY cgGetResourceString(CGresource resource)
	'CG_API CGresource CGENTRY cgGetResource(const char *resource_string)
	'CG_API const char * CGENTRY cgGetEnumString(CGenum en)
	'Function cgGetEnum(const char *enum_string)
	'CG_API const char * CGENTRY cgGetProfileString(CGprofile profile)
	'CG_API CGprofile CGENTRY cgGetProfile(const char *profile_string)
	'Function cgGetNumSupportedProfiles(void)
	'CG_API CGprofile CGENTRY cgGetSupportedProfile(int index)
	'Function cgIsProfileSupported(CGprofile profile)
	'Function cgGetProfileProperty(CGprofile profile, CGenum query)
	'CG_API const char * CGENTRY cgGetParameterClassString(CGparameterclass pc)
	'CG_API CGparameterclass CGENTRY cgGetParameterClassEnum(const char *pString)
	'CG_API const char * CGENTRY cgGetDomainString(CGdomain domain)
	'CG_API CGdomain CGENTRY cgGetDomain(const char *domain_string)
	'CG_API CGdomain CGENTRY cgGetProgramDomain(program)
	'CG_API CGerror CGENTRY cgGetError(void)
	'CG_API CGerror CGENTRY cgGetFirstError(void)
	'CG_API const char * CGENTRY cgGetErrorString(CGerror error)
	'CG_API const char * CGENTRY cgGetLastErrorString(CGerror *error)
	'Function cgSetErrorCallback(CGerrorCallbackFunc func)
	'CG_API CGerrorCallbackFunc CGENTRY cgGetErrorCallback(void)
	'Function cgSetErrorHandler(CGerrorHandlerFunc func, void *data)
	'CG_API CGerrorHandlerFunc CGENTRY cgGetErrorHandler(void **data)
	'CG_API const char * CGENTRY cgGetString(CGenum sname)
	'CG_API CGeffect CGENTRY cgCreateEffect(context, const char *code, const char **args)
	'CG_API CGeffect CGENTRY cgCreateEffectFromFile(context, const char *filename, const char **args)
	'CG_API CGeffect CGENTRY cgCopyEffect(CGeffect effect)
	'Function cgDestroyEffect(CGeffect effect)
	'CG_API CGcontext CGENTRY cgGetEffectContext(CGeffect effect)
	'Function cgIsEffect(CGeffect effect)
	'CG_API CGeffect CGENTRY cgGetFirstEffect(context)
	'CG_API CGeffect CGENTRY cgGetNextEffect(CGeffect effect)
	'Function cgCreateProgramFromEffect(CGeffect effect, CGprofile profile, const char *entry, const char **args)
	'CG_API CGtechnique CGENTRY cgGetFirstTechnique(CGeffect effect)
	'CG_API CGtechnique CGENTRY cgGetNextTechnique(CGtechnique tech)
	'CG_API CGtechnique CGENTRY cgGetNamedTechnique(CGeffect effect, const char *name)
	'CG_API const char * CGENTRY cgGetTechniqueName(CGtechnique tech)
	'Function cgIsTechnique(CGtechnique tech)
	'Function cgValidateTechnique(CGtechnique tech)
	'Function cgIsTechniqueValidated(CGtechnique tech)
	'CG_API CGeffect CGENTRY cgGetTechniqueEffect(CGtechnique tech)
	'CG_API CGpass CGENTRY cgGetFirstPass(CGtechnique tech)
	'CG_API CGpass CGENTRY cgGetNamedPass(CGtechnique tech, const char *name)
	'CG_API CGpass CGENTRY cgGetNextPass(CGpass pass)
	'Function cgIsPass(CGpass pass)
	'CG_API const char * CGENTRY cgGetPassName(CGpass pass)
	'CG_API CGtechnique CGENTRY cgGetPassTechnique(CGpass pass)
	'Function cgGetPassProgram(CGpass pass, CGdomain domain)
	'Function cgSetPassState(CGpass pass)
	'Function cgResetPassState(CGpass pass)
	'Function cgGetFirstStateAssignment(CGpass pass)
	'Function cgGetNamedStateAssignment(CGpass pass, const char *name)
	'Function cgGetNextStateAssignment(sa)
	'Function cgIsStateAssignment(sa)
	'Function cgCallStateSetCallback(sa)
	'Function cgCallStateValidateCallback(sa)
	'Function cgCallStateResetCallback(sa)
	'CG_API CGpass CGENTRY cgGetStateAssignmentPass(sa)
	'Function cgGetSamplerStateAssignmentParameter(sa)
	'CG_API const float * CGENTRY cgGetFloatStateAssignmentValues(sa, int *nvalues)
	'CG_API const int * CGENTRY cgGetIntStateAssignmentValues(sa, int *nvalues)
	'CG_API const CGbool * CGENTRY cgGetBoolStateAssignmentValues(sa, int *nvalues)
	'CG_API const char * CGENTRY cgGetStringStateAssignmentValue(sa)
	'Function cgGetProgramStateAssignmentValue(sa)
	'Function cgGetTextureStateAssignmentValue(sa)
	'Function cgGetSamplerStateAssignmentValue(sa)
	'Function cgGetStateAssignmentIndex(sa)
	'Function cgGetNumDependentStateAssignmentParameters(sa)
	'Function cgGetDependentStateAssignmentParameter(sa, int index)
	'Function cgGetNumDependentProgramArrayStateAssignmentParameters(sa)
	'Function cgGetDependentProgramArrayStateAssignmentParameter(sa, int index)
	'Function cgGetConnectedStateAssignmentParameter(sa)
	'CG_API CGstate CGENTRY cgGetStateAssignmentState(sa)
	'CG_API CGstate CGENTRY cgGetSamplerStateAssignmentState(sa)
	'CG_API CGstate CGENTRY cgCreateState(context, const char *name, CGtype type)
	'CG_API CGstate CGENTRY cgCreateArrayState(context, const char *name, CGtype type, int nelements)
	'Function cgSetStateCallbacks(CGstate state, CGstatecallback set, CGstatecallback reset, CGstatecallback validate)
	'Function cgSetStateLatestProfile(CGstate state, CGprofile profile)
	'CG_API CGstatecallback CGENTRY cgGetStateSetCallback(CGstate state)
	'CG_API CGstatecallback CGENTRY cgGetStateResetCallback(CGstate state)
	'CG_API CGstatecallback CGENTRY cgGetStateValidateCallback(CGstate state)
	'CG_API CGprofile CGENTRY cgGetStateLatestProfile(CGstate state)
	'CG_API CGcontext CGENTRY cgGetStateContext(CGstate state)
	'CG_API CGtype CGENTRY cgGetStateType(CGstate state)
	'CG_API const char * CGENTRY cgGetStateName(CGstate state)
	'CG_API CGstate CGENTRY cgGetNamedState(context, const char *name)
	'CG_API CGstate CGENTRY cgGetFirstState(context)
	'CG_API CGstate CGENTRY cgGetNextState(CGstate state)
	'Function cgIsState(CGstate state)
	'Function cgAddStateEnumerant(CGstate state, const char *name, int value)
	'CG_API CGstate CGENTRY cgCreateSamplerState(context, const char *name, CGtype type)
	'CG_API CGstate CGENTRY cgCreateArraySamplerState(context, const char *name, CGtype type, int nelements)
	'CG_API CGstate CGENTRY cgGetNamedSamplerState(context, const char *name)
	'CG_API CGstate CGENTRY cgGetFirstSamplerState(context)
	'Function cgGetFirstSamplerStateAssignment(param)
	'Function cgGetNamedSamplerStateAssignment(param, const char *name)
	'Function cgSetSamplerState(param)
	'Function cgGetNamedEffectParameter(CGeffect effect, const char *name)
	'Function cgGetFirstLeafEffectParameter(CGeffect effect)
	'Function cgGetFirstEffectParameter(CGeffect effect)
	'Function cgGetEffectParameterBySemantic(CGeffect effect, const char *semantic)
	'Function cgGetFirstTechniqueAnnotation(CGtechnique tech)
	'Function cgGetFirstPassAnnotation(CGpass pass)
	'Function cgGetFirstParameterAnnotation(param)
	'Function cgGetFirstProgramAnnotation(program)
	'Function cgGetFirstEffectAnnotation(CGeffect effect)
	'Function cgGetNextAnnotation(CGannotation ann)
	'Function cgGetNamedTechniqueAnnotation(CGtechnique tech, const char *name)
	'Function cgGetNamedPassAnnotation(CGpass pass, const char *name)
	'Function cgGetNamedParameterAnnotation(param, const char *name)
	'Function cgGetNamedProgramAnnotation(program, const char *name)
	'Function cgGetNamedEffectAnnotation(CGeffect effect, const char *name)
	'Function cgIsAnnotation(CGannotation ann)
	'CG_API const char * CGENTRY cgGetAnnotationName(CGannotation ann)
	'CG_API CGtype CGENTRY cgGetAnnotationType(CGannotation ann)
	'CG_API const float * CGENTRY cgGetFloatAnnotationValues(CGannotation ann, int *nvalues)
	'CG_API const int * CGENTRY cgGetIntAnnotationValues(CGannotation ann, int *nvalues)
	'CG_API const char * CGENTRY cgGetStringAnnotationValue(CGannotation ann)
	'CG_API const char * const * CGENTRY cgGetStringAnnotationValues(CGannotation ann, int *nvalues)
	'CG_API const CGbool * CGENTRY cgGetBoolAnnotationValues(CGannotation ann, int *nvalues)
	'CG_API const int * CGENTRY cgGetBooleanAnnotationValues(CGannotation ann, int *nvalues)
	'Function cgGetNumDependentAnnotationParameters(CGannotation ann)
	'Function cgGetDependentAnnotationParameter(CGannotation ann, int index)
	'Function cgEvaluateProgram(program, float *buf, int ncomps, int nx, int ny, int nz)
	'Function cgSetEffectName(CGeffect effect, const char *name)
	'CG_API const char * CGENTRY cgGetEffectName(CGeffect effect)
	'CG_API CGeffect CGENTRY cgGetNamedEffect(context, const char *name)
	'Function cgCreateEffectParameter(CGeffect effect, const char *name, CGtype type)
	'CG_API CGtechnique CGENTRY cgCreateTechnique(CGeffect effect, const char *name)
	'Function cgCreateEffectParameterArray(CGeffect effect, const char *name, CGtype type, int length)
	'Function cgCreateEffectParameterMultiDimArray(CGeffect effect, const char *name, CGtype type, int dim, const int *lengths)
	'CG_API CGpass CGENTRY cgCreatePass(CGtechnique tech, const char *name)
	'Function cgCreateStateAssignment(CGpass pass, CGstate state)
	'Function cgCreateStateAssignmentIndex(CGpass pass, CGstate state, int index)
	'Function cgCreateSamplerStateAssignment(param, CGstate state)
	'Function cgSetFloatStateAssignment(sa, float value)
	'Function cgSetIntStateAssignment(sa, int value)
	'Function cgSetBoolStateAssignment(sa, CGbool value)
	'Function cgSetStringStateAssignment(sa, const char *value)
	'Function cgSetProgramStateAssignment(sa, program)
	'Function cgSetSamplerStateAssignment(sa, param)
	'Function cgSetTextureStateAssignment(sa, param)
	'Function cgSetFloatArrayStateAssignment(sa, const float *vals)
	'Function cgSetIntArrayStateAssignment(sa, const int *vals)
	'Function cgSetBoolArrayStateAssignment(sa, const CGbool *vals)
	'Function cgCreateTechniqueAnnotation(CGtechnique tech, const char *name, CGtype type)
	'Function cgCreatePassAnnotation(CGpass pass, const char *name, CGtype type)
	'Function cgCreateParameterAnnotation(param, const char *name, CGtype type)
	'Function cgCreateProgramAnnotation(program, const char *name, CGtype type)
	'Function cgCreateEffectAnnotation(CGeffect effect, const char *name, CGtype type)
	'Function cgSetIntAnnotation(CGannotation ann, int value)
	'Function cgSetFloatAnnotation(CGannotation ann, float value)
	'Function cgSetBoolAnnotation(CGannotation ann, CGbool value)
	'Function cgSetStringAnnotation(CGannotation ann, const char *value)
	'Function cgGetNumStateEnumerants(CGstate state)
	'CG_API const char * CGENTRY cgGetStateEnumerant(CGstate state, int index, int *value)
	'CG_API const char * CGENTRY cgGetStateEnumerantName(CGstate state, int value)
	'Function cgGetStateEnumerantValue(CGstate state, const char *name)
	'CG_API CGeffect CGENTRY cgGetParameterEffect(param)
	'CG_API CGparameterclass CGENTRY cgGetTypeClass(CGtype type)
	'CG_API CGtype CGENTRY cgGetTypeBase(CGtype type)
	'Function cgGetTypeSizes(CGtype type, int *nrows, int *ncols)
	'Function cgGetMatrixSize(CGtype type, int *nrows, int *ncols)
	'Function cgGetNumProgramDomains(program)
	'CG_API CGdomain CGENTRY cgGetProfileDomain(CGprofile profile)
	'Function cgCombinePrograms(int n, const CGprogram *exeList)
	'Function cgCombinePrograms2(const CGprogram exe1, const CGprogram exe2)
	'Function cgCombinePrograms3(const CGprogram exe1, const CGprogram exe2, const CGprogram exe3)
	'Function cgCombinePrograms4(const CGprogram exe1, const CGprogram exe2, const CGprogram exe3, const CGprogram exe4)
	'Function cgCombinePrograms5(const CGprogram exe1, const CGprogram exe2, const CGprogram exe3, const CGprogram exe4, const CGprogram exe5)
	'CG_API CGprofile CGENTRY cgGetProgramDomainProfile(program, int index)
	'Function cgGetProgramDomainProgram(program, int index)
	'CG_API CGobj CGENTRY cgCreateObj(context, CGenum program_type, const char *source, CGprofile profile, const char **args)
	'CG_API CGobj CGENTRY cgCreateObjFromFile(context, CGenum program_type, const char *source_file, CGprofile profile, const char **args)
	'Function cgDestroyObj(CGobj obj)
	'CG_API long CGENTRY cgGetParameterResourceSize(param)
	'CG_API CGtype CGENTRY cgGetParameterResourceType(param)
	'CG_API const char * CGENTRY cgGetParameterResourceName(param)
	'Function cgGetParameterBufferIndex(param)
	'Function cgGetParameterBufferOffset(param)
	'CG_API CGbuffer CGENTRY cgCreateBuffer(context, int size, const void *data, CGbufferusage bufferUsage)
	'Function cgSetBufferData(CGbuffer buffer, int size, const void *data)
	'Function cgSetBufferSubData(CGbuffer buffer, int offset, int size, const void *data)
	'Function cgSetProgramBuffer(program, int bufferIndex, CGbuffer buffer)
	'CG_API void * CGENTRY cgMapBuffer(CGbuffer buffer, CGbufferaccess access)
	'Function cgUnmapBuffer(CGbuffer buffer)
	'Function cgDestroyBuffer(CGbuffer buffer)
	'CG_API CGbuffer CGENTRY cgGetProgramBuffer(program, int bufferIndex)
	'Function cgGetBufferSize(CGbuffer buffer)
	'Function cgGetProgramBufferMaxSize(CGprofile profile)
	'Function cgGetProgramBufferMaxIndex(CGprofile profile)
	'Function cgGetEffectParameterBuffer(param)
	'Function cgSetEffectParameterBuffer(param, CGbuffer buffer)
End Extern