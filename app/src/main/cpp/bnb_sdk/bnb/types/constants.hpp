#pragma once

#include <stdint.h>

namespace bnb
{
    /** 
     * @addtogroup types
     * @{
     */
    namespace constants
    {
        constexpr uint32_t FULL_FRAME_W = 720;
        constexpr uint32_t FULL_FRAME_H = 1280;
        constexpr uint32_t FULL_FRAME_SIZE = FULL_FRAME_W * FULL_FRAME_H;

        constexpr uint32_t FRX_FRAME_W = 480;
        constexpr uint32_t FRX_FRAME_H = 640;
        constexpr uint32_t FRX_FRAME_SIZE = FRX_FRAME_W * FRX_FRAME_H;

        constexpr uint32_t NEURO_FRX_LATS_CNT = 36;
        constexpr uint32_t NEURO_FRX_LNDS_CNT = 68;
        constexpr uint32_t NEURO_FRX_VERTS_CNT = 3308;
        constexpr uint32_t NEURO_FRX_TRIANGLES_CNT = 19308;

        // TODO add support for reduced
#define FULL_IBUG_68 1

        // clang-format off

        enum landmark_point_id
        {
#if FULL_IBUG_68
            X_1_FACE = 0, Y_1_FACE,
            X_2_FACE, Y_2_FACE,
#else
            X_2_FACE = 0,          Y_2_FACE = 0,
#endif
            X_3_FACE, Y_3_FACE,
            X_4_FACE, Y_4_FACE,
            X_5_FACE, Y_5_FACE,
            X_6_FACE, Y_6_FACE,
            X_7_FACE, Y_7_FACE,
            X_8_L_CHIN, Y_8_L_CHIN,
            X_9_CHIN, Y_9_CHIN,
            X_10_R_CHIN, Y_10_R_CHIN,
            X_11_FACE, Y_11_FACE,
            X_12_FACE, Y_12_FACE,
            X_13_FACE, Y_13_FACE,
            X_14_FACE, Y_14_FACE,
            X_15_FACE, Y_15_FACE,
            X_16_FACE, Y_16_FACE,
#if FULL_IBUG_68
            X_17_FACE, Y_17_FACE,
#endif
            X_18_L_BROW, Y_18_L_BROW,
            X_19_L_BROW, Y_19_L_BROW,
            X_20_L_BROW, Y_20_L_BROW,
            X_21_L_BROW, Y_21_L_BROW,
            X_22_L_BROW, Y_22_L_BROW,
            X_23_R_BROW, Y_23_R_BROW,
            X_24_R_BROW, Y_24_R_BROW,
            X_25_R_BROW, Y_25_R_BROW,
            X_26_R_BROW, Y_26_R_BROW,
            X_27_R_BROW, Y_27_R_BROW,
            X_28_NOSE_BRIDGE, Y_28_NOSE_BRIDGE,
            X_29_NOSE_BRIDGE, Y_29_NOSE_BRIDGE,
            X_30_NOSE_BRIDGE, Y_30_NOSE_BRIDGE,
            X_31_NOSE_BRIDGE, Y_31_NOSE_BRIDGE,
            X_32_NOSE, Y_32_NOSE,
            X_33_NOSE, Y_33_NOSE,
            X_34_NOSE, Y_34_NOSE,
            X_35_NOSE, Y_35_NOSE,
            X_36_NOSE, Y_36_NOSE,
            X_37_L_EYE_L_CORNER, Y_37_L_EYE_L_CORNER,
            X_38_LU_EYELID, Y_38_LU_EYELID,
            X_39_LU_EYELID, Y_39_LU_EYELID,
            X_40_L_EYE_R_CORNER, Y_40_L_EYE_R_CORNER,
            X_41_LD_EYELID, Y_41_LD_EYELID,
            X_42_LD_EYELID, Y_42_LD_EYELID,
            X_43_R_EYE_L_CORNER, Y_43_R_EYE_L_CORNER,
            X_44_RU_EYELID, Y_44_RU_EYELID,
            X_45_RU_EYELID, Y_45_RU_EYELID,
            X_46_R_EYE_R_CORNER, Y_46_R_EYE_R_CORNER,
            X_47_RD_EYELID, Y_47_RD_EYELID,
            X_48_RD_EYELID, Y_48_RD_EYELID,
            X_49_LIP_L_CORNER_OUT, Y_49_LIP_L_CORNER_OUT,
            X_50_LIP_U_OUT, Y_50_LIP_U_OUT,
            X_51_LIP_U_OUT, Y_51_LIP_U_OUT,
            X_52_LIP_U_OUT, Y_52_LIP_U_OUT,
            X_53_LIP_U_OUT, Y_53_LIP_U_OUT,
            X_54_LIP_U_OUT, Y_54_LIP_U_OUT,
            X_55_LIP_R_CORNER_OUT, Y_55_LIP_R_CORNER_OUT,
            X_56_LIP_D_OUT, Y_56_LIP_D_OUT,
            X_57_LIP_D_OUT, Y_57_LIP_D_OUT,
            X_58_LIP_D_OUT, Y_58_LIP_D_OUT,
            X_59_LIP_D_OUT, Y_59_LIP_D_OUT,
            X_60_LIP_D_OUT, Y_60_LIP_D_OUT,
#if FULL_IBUG_68
            X_61_LIP_L_CORNER_OUT, Y_61_LIP_L_CORNER_OUT,
#endif
            X_62_LIP_U_IN, Y_62_LIP_U_IN,
            X_63_LIP_U_IN, Y_63_LIP_U_IN,
            X_64_LIP_U_IN, Y_64_LIP_U_IN,
#if FULL_IBUG_68
            X_65_LIP_R_CORNER_OUT, Y_65_LIP_R_CORNER_OUT,
#endif
            X_66_LIP_D_IN, Y_66_LIP_D_IN,
            X_67_LIP_D_IN, Y_67_LIP_D_IN,
            X_68_LIP_D_IN, Y_68_LIP_D_IN,
            END,
        };

        // clang-format on
    }         // namespace constants
    /** @} */ // endgroup types
} // namespace bnb
