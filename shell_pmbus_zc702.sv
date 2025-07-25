`default_nettype none
`timescale 1ns/1ps

module shell_pmbus_zc702 (

    // DDR3 PS (constraints inherited from the board file)

        inout wire [15-1:0] DDR_addr,
        inout wire [3-1:0] DDR_ba,
        inout wire DDR_cas_n,
        inout wire DDR_ck_n,
        inout wire DDR_ck_p,
        inout wire DDR_cke,
        inout wire DDR_cs_n,
        inout wire [4-1:0] DDR_dm,
        inout wire [32-1:0] DDR_dq,
        inout wire [4-1:0] DDR_dqs_n,
        inout wire [4-1:0] DDR_dqs_p,
        inout wire DDR_odt,
        inout wire DDR_ras_n,
        inout wire DDR_reset_n,
        inout wire DDR_we_n,

    // Misc IOs (constraints inherited from the board file)

        inout wire FIXED_IO_ddr_vrn,
        inout wire FIXED_IO_ddr_vrp,
        inout wire [54-1:0] FIXED_IO_mio,
        inout wire FIXED_IO_ps_clk,
        inout wire FIXED_IO_ps_porb,
        inout wire FIXED_IO_ps_srstb

);

bd_ps bd_ps_i (

    .DDR_addr(DDR_addr),
    .DDR_ba(DDR_ba),
    .DDR_cas_n(DDR_cas_n),
    .DDR_ck_n(DDR_ck_n),
    .DDR_ck_p(DDR_ck_p),
    .DDR_cke(DDR_cke),
    .DDR_cs_n(DDR_cs_n),
    .DDR_dm(DDR_dm),
    .DDR_dq(DDR_dq),
    .DDR_dqs_n(DDR_dqs_n),
    .DDR_dqs_p(DDR_dqs_p),
    .DDR_odt(DDR_odt),
    .DDR_ras_n(DDR_ras_n),
    .DDR_reset_n(DDR_reset_n),
    .DDR_we_n(DDR_we_n),

    .FIXED_IO_ddr_vrn(FIXED_IO_ddr_vrn),
    .FIXED_IO_ddr_vrp(FIXED_IO_ddr_vrp),
    .FIXED_IO_mio(FIXED_IO_mio),
    .FIXED_IO_ps_clk(FIXED_IO_ps_clk),
    .FIXED_IO_ps_porb(FIXED_IO_ps_porb),
    .FIXED_IO_ps_srstb(FIXED_IO_ps_srstb)

);

endmodule

`resetall
