// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Transpose an array of \c __m128i (treating it as a square matrix of integers) using SSE2 intrinsics
/**
\file
\author Steven Ward
*/

#pragma once

#include "simd-array.hpp"

#include <immintrin.h>

static inline void
transpose([[maybe_unused]] arr_m128i<1>& x)
{
    // NOP
}

/// Transpose \a x (treating it as a 2x2 matrix of \c uint64_t) using SSE2 intrinsics
static void
transpose(arr_m128i<2>& x)
{
    const __m128i A0_B0 = _mm_unpacklo_epi64(x[0], x[1]);
    const __m128i A1_B1 = _mm_unpackhi_epi64(x[0], x[1]);

    x[0] = A0_B0;
    x[1] = A1_B1;
}

/// Transpose \a x (treating it as a 4x4 matrix of \c uint32_t) using SSE2 intrinsics
/**
\sa https://randombit.net/bitbashing/posts/integer_matrix_transpose_in_sse2.html
*/
static void
transpose(arr_m128i<4>& x)
{
    const __m128i A01_B01 = _mm_unpacklo_epi32(x[0], x[1]);
    const __m128i A23_B23 = _mm_unpackhi_epi32(x[0], x[1]);
    const __m128i C01_D01 = _mm_unpacklo_epi32(x[2], x[3]);
    const __m128i C23_D23 = _mm_unpackhi_epi32(x[2], x[3]);

    x[0] = _mm_unpacklo_epi64(A01_B01, C01_D01); // A0_B0_C0_D0
    x[1] = _mm_unpackhi_epi64(A01_B01, C01_D01); // A1_B1_C1_D1
    x[2] = _mm_unpacklo_epi64(A23_B23, C23_D23); // A2_B2_C2_D2
    x[3] = _mm_unpackhi_epi64(A23_B23, C23_D23); // A3_B3_C3_D3
}

/// Transpose \a x (treating it as a 4x4 matrix of \c uint32_t) using the \c _MM_TRANSPOSE4_PS macro
/**
\sa https://community.intel.com/t5/Intel-ISA-Extensions/4x4-matrix-transpose-using-sse2-intrinsics/m-p/785883#M353
\verbatim
Cast your __m128i variables into __m128 variables (using _mm_castsi128_ps), use the macro _MM_TRANSPOSE_PS, then cast back using _mm_castps_si128.
\endverbatim
*/
static void
transpose_macro(arr_m128i<4>& x)
{
    __m128 A = _mm_castsi128_ps(x[0]);
    __m128 B = _mm_castsi128_ps(x[1]);
    __m128 C = _mm_castsi128_ps(x[2]);
    __m128 D = _mm_castsi128_ps(x[3]);

    _MM_TRANSPOSE4_PS(A, B, C, D);

    x[0] = _mm_castps_si128(A);
    x[1] = _mm_castps_si128(B);
    x[2] = _mm_castps_si128(C);
    x[3] = _mm_castps_si128(D);
}

/// Transpose \a x (treating it as an 8x8 matrix of \c uint16_t) using SSE2 intrinsics
/**
\sa https://stackoverflow.com/a/4951060/1892784
*/
static void
transpose(arr_m128i<8>& x)
{
    const __m128i A03_B03 = _mm_unpacklo_epi16(x[0], x[1]);
    const __m128i A47_B47 = _mm_unpackhi_epi16(x[0], x[1]);
    const __m128i C03_D03 = _mm_unpacklo_epi16(x[2], x[3]);
    const __m128i C47_D47 = _mm_unpackhi_epi16(x[2], x[3]);
    const __m128i E03_F03 = _mm_unpacklo_epi16(x[4], x[5]);
    const __m128i E47_F47 = _mm_unpackhi_epi16(x[4], x[5]);
    const __m128i G03_H03 = _mm_unpacklo_epi16(x[6], x[7]);
    const __m128i G47_H47 = _mm_unpackhi_epi16(x[6], x[7]);

    const __m128i A01_B01_C01_D01 = _mm_unpacklo_epi32(A03_B03, C03_D03);
    const __m128i A23_B23_C23_D23 = _mm_unpackhi_epi32(A03_B03, C03_D03);
    const __m128i E01_F01_G01_H01 = _mm_unpacklo_epi32(E03_F03, G03_H03);
    const __m128i E23_F23_G23_H23 = _mm_unpackhi_epi32(E03_F03, G03_H03);
    const __m128i A45_B45_C45_D45 = _mm_unpacklo_epi32(A47_B47, C47_D47);
    const __m128i A67_B67_C67_D67 = _mm_unpackhi_epi32(A47_B47, C47_D47);
    const __m128i E45_F45_G45_H45 = _mm_unpacklo_epi32(E47_F47, G47_H47);
    const __m128i E67_F67_G67_H67 = _mm_unpackhi_epi32(E47_F47, G47_H47);

    x[0] = _mm_unpacklo_epi64(A01_B01_C01_D01, E01_F01_G01_H01); // A0_B0_C0_D0_E0_F0_G0_H0
    x[1] = _mm_unpackhi_epi64(A01_B01_C01_D01, E01_F01_G01_H01); // A1_B1_C1_D1_E1_F1_G1_H1
    x[2] = _mm_unpacklo_epi64(A23_B23_C23_D23, E23_F23_G23_H23); // A2_B2_C2_D2_E2_F2_G2_H2
    x[3] = _mm_unpackhi_epi64(A23_B23_C23_D23, E23_F23_G23_H23); // A3_B3_C3_D3_E3_F3_G3_H3
    x[4] = _mm_unpacklo_epi64(A45_B45_C45_D45, E45_F45_G45_H45); // A4_B4_C4_D4_E4_F4_G4_H4
    x[5] = _mm_unpackhi_epi64(A45_B45_C45_D45, E45_F45_G45_H45); // A5_B5_C5_D5_E5_F5_G5_H5
    x[6] = _mm_unpacklo_epi64(A67_B67_C67_D67, E67_F67_G67_H67); // A6_B6_C6_D6_E6_F6_G6_H6
    x[7] = _mm_unpackhi_epi64(A67_B67_C67_D67, E67_F67_G67_H67); // A7_B7_C7_D7_E7_F7_G7_H7
}

/// Transpose \a x (treating it as a 16x16 matrix of \c uint8_t) using SSE2 intrinsics
/**
\sa https://codereview.stackexchange.com/questions/295941/16x16-integer-matrix-transpose-using-sse2-intrinsics-in-c
*/
static void
transpose(arr_m128i<16>& x)
{
    const __m128i A07_B07 = _mm_unpacklo_epi8(x[0x0], x[0x1]);
    const __m128i A8f_B8f = _mm_unpackhi_epi8(x[0x0], x[0x1]);
    const __m128i C07_D07 = _mm_unpacklo_epi8(x[0x2], x[0x3]);
    const __m128i C8f_D8f = _mm_unpackhi_epi8(x[0x2], x[0x3]);
    const __m128i E07_F07 = _mm_unpacklo_epi8(x[0x4], x[0x5]);
    const __m128i E8f_F8f = _mm_unpackhi_epi8(x[0x4], x[0x5]);
    const __m128i G07_H07 = _mm_unpacklo_epi8(x[0x6], x[0x7]);
    const __m128i G8f_H8f = _mm_unpackhi_epi8(x[0x6], x[0x7]);
    const __m128i I07_J07 = _mm_unpacklo_epi8(x[0x8], x[0x9]);
    const __m128i I8f_J8f = _mm_unpackhi_epi8(x[0x8], x[0x9]);
    const __m128i K07_L07 = _mm_unpacklo_epi8(x[0xa], x[0xb]);
    const __m128i K8f_L8f = _mm_unpackhi_epi8(x[0xa], x[0xb]);
    const __m128i M07_N07 = _mm_unpacklo_epi8(x[0xc], x[0xd]);
    const __m128i M8f_N8f = _mm_unpackhi_epi8(x[0xc], x[0xd]);
    const __m128i O07_P07 = _mm_unpacklo_epi8(x[0xe], x[0xf]);
    const __m128i O8f_P8f = _mm_unpackhi_epi8(x[0xe], x[0xf]);

    const __m128i A03_B03_C03_D03 = _mm_unpacklo_epi16(A07_B07, C07_D07);
    const __m128i A47_B47_C47_D47 = _mm_unpackhi_epi16(A07_B07, C07_D07);
    const __m128i A8b_B8b_C8b_D8b = _mm_unpacklo_epi16(A8f_B8f, C8f_D8f);
    const __m128i Acf_Bcf_Ccf_Dcf = _mm_unpackhi_epi16(A8f_B8f, C8f_D8f);
    const __m128i E03_F03_G03_H03 = _mm_unpacklo_epi16(E07_F07, G07_H07);
    const __m128i E47_F47_G47_H47 = _mm_unpackhi_epi16(E07_F07, G07_H07);
    const __m128i E8b_F8b_G8b_H8b = _mm_unpacklo_epi16(E8f_F8f, G8f_H8f);
    const __m128i Ecf_Fcf_Gcf_Hcf = _mm_unpackhi_epi16(E8f_F8f, G8f_H8f);
    const __m128i I03_J03_K03_L03 = _mm_unpacklo_epi16(I07_J07, K07_L07);
    const __m128i I47_J47_K47_L47 = _mm_unpackhi_epi16(I07_J07, K07_L07);
    const __m128i I8b_J8b_K8b_L8b = _mm_unpacklo_epi16(I8f_J8f, K8f_L8f);
    const __m128i Icf_Jcf_Kcf_Lcf = _mm_unpackhi_epi16(I8f_J8f, K8f_L8f);
    const __m128i M03_N03_O03_P03 = _mm_unpacklo_epi16(M07_N07, O07_P07);
    const __m128i M47_N47_O47_P47 = _mm_unpackhi_epi16(M07_N07, O07_P07);
    const __m128i M8b_N8b_O8b_P8b = _mm_unpacklo_epi16(M8f_N8f, O8f_P8f);
    const __m128i Mcf_Ncf_Ocf_Pcf = _mm_unpackhi_epi16(M8f_N8f, O8f_P8f);

    const __m128i A01_B01_C01_D01_E01_F01_G01_H01 = _mm_unpacklo_epi32(A03_B03_C03_D03, E03_F03_G03_H03);
    const __m128i A23_B23_C23_D23_E23_F23_G23_H23 = _mm_unpackhi_epi32(A03_B03_C03_D03, E03_F03_G03_H03);
    const __m128i I01_J01_K01_L01_M01_N01_O01_P01 = _mm_unpacklo_epi32(I03_J03_K03_L03, M03_N03_O03_P03);
    const __m128i I23_J23_K23_L23_M23_N23_O23_P23 = _mm_unpackhi_epi32(I03_J03_K03_L03, M03_N03_O03_P03);
    const __m128i A45_B45_C45_D45_E45_F45_G45_H45 = _mm_unpacklo_epi32(A47_B47_C47_D47, E47_F47_G47_H47);
    const __m128i A67_B67_C67_D67_E67_F67_G67_H67 = _mm_unpackhi_epi32(A47_B47_C47_D47, E47_F47_G47_H47);
    const __m128i I45_J45_K45_L45_M45_N45_O45_P45 = _mm_unpacklo_epi32(I47_J47_K47_L47, M47_N47_O47_P47);
    const __m128i I67_J67_K67_L67_M67_N67_O67_P67 = _mm_unpackhi_epi32(I47_J47_K47_L47, M47_N47_O47_P47);
    const __m128i A89_B89_C89_D89_E89_F89_G89_H89 = _mm_unpacklo_epi32(A8b_B8b_C8b_D8b, E8b_F8b_G8b_H8b);
    const __m128i Aab_Bab_Cab_Dab_Eab_Fab_Gab_Hab = _mm_unpackhi_epi32(A8b_B8b_C8b_D8b, E8b_F8b_G8b_H8b);
    const __m128i I89_J89_K89_L89_M89_N89_O89_P89 = _mm_unpacklo_epi32(I8b_J8b_K8b_L8b, M8b_N8b_O8b_P8b);
    const __m128i Iab_Jab_Kab_Lab_Mab_Nab_Oab_Pab = _mm_unpackhi_epi32(I8b_J8b_K8b_L8b, M8b_N8b_O8b_P8b);
    const __m128i Acd_Bcd_Ccd_Dcd_Ecd_Fcd_Gcd_Hcd = _mm_unpacklo_epi32(Acf_Bcf_Ccf_Dcf, Ecf_Fcf_Gcf_Hcf);
    const __m128i Aef_Bef_Cef_Def_Eef_Fef_Gef_Hef = _mm_unpackhi_epi32(Acf_Bcf_Ccf_Dcf, Ecf_Fcf_Gcf_Hcf);
    const __m128i Icd_Jcd_Kcd_Lcd_Mcd_Ncd_Ocd_Pcd = _mm_unpacklo_epi32(Icf_Jcf_Kcf_Lcf, Mcf_Ncf_Ocf_Pcf);
    const __m128i Ief_Jef_Kef_Lef_Mef_Nef_Oef_Pef = _mm_unpackhi_epi32(Icf_Jcf_Kcf_Lcf, Mcf_Ncf_Ocf_Pcf);

    x[0x0] = _mm_unpacklo_epi64(A01_B01_C01_D01_E01_F01_G01_H01, I01_J01_K01_L01_M01_N01_O01_P01); // A0_B0_C0_D0_E0_F0_G0_H0_I0_J0_K0_L0_M0_N0_O0_P0
    x[0x1] = _mm_unpackhi_epi64(A01_B01_C01_D01_E01_F01_G01_H01, I01_J01_K01_L01_M01_N01_O01_P01); // A1_B1_C1_D1_E1_F1_G1_H1_I1_J1_K1_L1_M1_N1_O1_P1
    x[0x2] = _mm_unpacklo_epi64(A23_B23_C23_D23_E23_F23_G23_H23, I23_J23_K23_L23_M23_N23_O23_P23); // A2_B2_C2_D2_E2_F2_G2_H2_I2_J2_K2_L2_M2_N2_O2_P2
    x[0x3] = _mm_unpackhi_epi64(A23_B23_C23_D23_E23_F23_G23_H23, I23_J23_K23_L23_M23_N23_O23_P23); // A3_B3_C3_D3_E3_F3_G3_H3_I3_J3_K3_L3_M3_N3_O3_P3
    x[0x4] = _mm_unpacklo_epi64(A45_B45_C45_D45_E45_F45_G45_H45, I45_J45_K45_L45_M45_N45_O45_P45); // A4_B4_C4_D4_E4_F4_G4_H4_I4_J4_K4_L4_M4_N4_O4_P4
    x[0x5] = _mm_unpackhi_epi64(A45_B45_C45_D45_E45_F45_G45_H45, I45_J45_K45_L45_M45_N45_O45_P45); // A5_B5_C5_D5_E5_F5_G5_H5_I5_J5_K5_L5_M5_N5_O5_P5
    x[0x6] = _mm_unpacklo_epi64(A67_B67_C67_D67_E67_F67_G67_H67, I67_J67_K67_L67_M67_N67_O67_P67); // A6_B6_C6_D6_E6_F6_G6_H6_I6_J6_K6_L6_M6_N6_O6_P6
    x[0x7] = _mm_unpackhi_epi64(A67_B67_C67_D67_E67_F67_G67_H67, I67_J67_K67_L67_M67_N67_O67_P67); // A7_B7_C7_D7_E7_F7_G7_H7_I7_J7_K7_L7_M7_N7_O7_P7
    x[0x8] = _mm_unpacklo_epi64(A89_B89_C89_D89_E89_F89_G89_H89, I89_J89_K89_L89_M89_N89_O89_P89); // A8_B8_C8_D8_E8_F8_G8_H8_I8_J8_K8_L8_M8_N8_O8_P8
    x[0x9] = _mm_unpackhi_epi64(A89_B89_C89_D89_E89_F89_G89_H89, I89_J89_K89_L89_M89_N89_O89_P89); // A9_B9_C9_D9_E9_F9_G9_H9_I9_J9_K9_L9_M9_N9_O9_P9
    x[0xa] = _mm_unpacklo_epi64(Aab_Bab_Cab_Dab_Eab_Fab_Gab_Hab, Iab_Jab_Kab_Lab_Mab_Nab_Oab_Pab); // Aa_Ba_Ca_Da_Ea_Fa_Ga_Ha_Ia_Ja_Ka_La_Ma_Na_Oa_Pa
    x[0xb] = _mm_unpackhi_epi64(Aab_Bab_Cab_Dab_Eab_Fab_Gab_Hab, Iab_Jab_Kab_Lab_Mab_Nab_Oab_Pab); // Ab_Bb_Cb_Db_Eb_Fb_Gb_Hb_Ib_Jb_Kb_Lb_Mb_Nb_Ob_Pb
    x[0xc] = _mm_unpacklo_epi64(Acd_Bcd_Ccd_Dcd_Ecd_Fcd_Gcd_Hcd, Icd_Jcd_Kcd_Lcd_Mcd_Ncd_Ocd_Pcd); // Ac_Bc_Cc_Dc_Ec_Fc_Gc_Hc_Ic_Jc_Kc_Lc_Mc_Nc_Oc_Pc
    x[0xd] = _mm_unpackhi_epi64(Acd_Bcd_Ccd_Dcd_Ecd_Fcd_Gcd_Hcd, Icd_Jcd_Kcd_Lcd_Mcd_Ncd_Ocd_Pcd); // Ad_Bd_Cd_Dd_Ed_Fd_Gd_Hd_Id_Jd_Kd_Ld_Md_Nd_Od_Pd
    x[0xe] = _mm_unpacklo_epi64(Aef_Bef_Cef_Def_Eef_Fef_Gef_Hef, Ief_Jef_Kef_Lef_Mef_Nef_Oef_Pef); // Ae_Be_Ce_De_Ee_Fe_Ge_He_Ie_Je_Ke_Le_Me_Ne_Oe_Pe
    x[0xf] = _mm_unpackhi_epi64(Aef_Bef_Cef_Def_Eef_Fef_Gef_Hef, Ief_Jef_Kef_Lef_Mef_Nef_Oef_Pef); // Af_Bf_Cf_Df_Ef_Ff_Gf_Hf_If_Jf_Kf_Lf_Mf_Nf_Of_Pf
}
