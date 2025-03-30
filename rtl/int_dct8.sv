/* verilator lint_off WIDTH */

module int_dct8 #(
  parameter int IN_WIDTH  = 8,
  parameter int CONST_WIDTH = 16,
  parameter int OUT_WIDTH   = 20
) (
  input  logic                         clk,
  input  logic                         rst,
  input  logic                         valid_in,
  input  logic signed [IN_WIDTH-1:0] x [0:7],
  output logic                         valid_out,
  output logic signed [OUT_WIDTH-1:0]  y [0:7]
);

  // Scale factors in Q15 format:
  // Ci = cos(i*pi/2*N) and Si = sin(i*pi/2*N)
  localparam signed [CONST_WIDTH-1:0] C3 = 16'sd27246;
  localparam signed [CONST_WIDTH-1:0] S3 = 16'sd18205;
  localparam signed [CONST_WIDTH-1:0] C4 = 16'sd23170;
  localparam signed [CONST_WIDTH-1:0] C6 = 16'sd12540;
  localparam signed [CONST_WIDTH-1:0] S6 = 16'sd30274;
  localparam signed [CONST_WIDTH-1:0] C7 = 16'sd6393;
  localparam signed [CONST_WIDTH-1:0] S7 = 16'sd32138;

  // Stage 1
  logic       s1_valid;
  logic signed [IN_WIDTH:0] s1_even [0:3];
  logic signed [IN_WIDTH:0] s1_odd  [0:3];

  always_ff @(posedge clk) begin
    if (rst) begin
      s1_valid <= 1'b0;
      for (int i = 0; i < 4; i++) begin
        s1_even[i] <= '0;
        s1_odd[i]  <= '0;
      end
    end else begin
      s1_valid   <= valid_in;
      s1_even[0] <= x[0] + x[7];
      s1_even[1] <= x[1] + x[6];
      s1_even[2] <= x[2] + x[5];
      s1_even[3] <= x[3] + x[4];
      s1_odd[0]  <= x[3] - x[4];
      s1_odd[1]  <= x[2] - x[5];
      s1_odd[2]  <= x[1] - x[6];
      s1_odd[3]  <= x[0] - x[7];
    end
  end

  // Stage 2
  logic       s2_valid;
  logic signed [IN_WIDTH+1:0] s2_even [0:3];
  logic signed [IN_WIDTH+CONST_WIDTH+1:0] s2_odd [0:3];

  always_ff @(posedge clk) begin
    if (rst) begin
      s2_valid <= 1'b0;
      for (int i = 0; i < 4; i++) begin
        s2_even[i] <= '0;
        s2_odd[i]  <= '0;
      end
    end else begin
      s2_valid   <= s1_valid;
      s2_even[0] <= s1_even[0] + s1_even[3];
      s2_even[1] <= s1_even[1] + s1_even[2];
      s2_even[2] <= s1_even[1] - s1_even[2];
      s2_even[3] <= s1_even[0] - s1_even[3];
      s2_odd[0]  <= s1_odd[0] << 15;  // Q15
      s2_odd[1]  <= (s1_odd[2] - s1_odd[1]) * C4;
      s2_odd[2]  <= (s1_odd[2] + s1_odd[1]) * C4;
      s2_odd[3]  <= s1_odd[3] << 15;
    end
  end

  // Stage 3
  logic       s3_valid;
  logic signed [IN_WIDTH+CONST_WIDTH+2:0] s3_even [0:3];
  logic signed [IN_WIDTH+CONST_WIDTH+2:0] s3_odd [0:3];

  always_ff @(posedge clk) begin
    if (rst) begin
      s3_valid <= 1'b0;
      for (int i = 0; i < 4; i++) begin
        s3_even[i] <= '0;
        s3_odd[i]  <= '0;
      end
    end else begin
      s3_valid   <= s2_valid;
      s3_even[0] <= (s2_even[0] + s2_even[1]) * C4;
      s3_even[1] <= (s2_even[0] - s2_even[1]) * C4;
      s3_even[2] <= s2_even[3] * S6 + s2_even[2] * C6;
      s3_even[3] <= s2_even[3] * C6 - s2_even[2] * S6;
      s3_odd[0]  <= s2_odd[0] + s2_odd[1];
      s3_odd[1]  <= s2_odd[0] - s2_odd[1];
      s3_odd[2]  <= s2_odd[3] - s2_odd[2];
      s3_odd[3]  <= s2_odd[3] + s2_odd[2];
    end
  end

  // Stage 4
  logic       s4_valid;
  logic signed [IN_WIDTH+2*CONST_WIDTH+3:0] s4_odd [0:3];

  always_ff @(posedge clk) begin
    if (rst) begin
      s4_valid <= 1'b0;
      for (int i = 0; i < 4; i++) begin
        s4_odd[i] <= '0;
      end
    end else begin
      s4_valid  <= s3_valid;
      s4_odd[0] <= (s3_odd[3] * S7 + s3_odd[0] * C7) >>> 15;
      s4_odd[1] <= (s3_odd[2] * S3 + s3_odd[1] * C3) >>> 15;
      s4_odd[2] <= (s3_odd[2] * C3 - s3_odd[1] * S3) >>> 15;
      s4_odd[3] <= (s3_odd[3] * C7 - s3_odd[0] * S7) >>> 15;
    end
  end

  // Q6 output
  assign valid_out = s4_valid;
  assign y[0] = s3_even[0] >>> 9;
  assign y[1] = s4_odd[0]  >>> 9;
  assign y[2] = s3_even[2] >>> 9;
  assign y[3] = s4_odd[2]  >>> 9;
  assign y[4] = s3_even[1] >>> 9;
  assign y[5] = s4_odd[1]  >>> 9;
  assign y[6] = s3_even[3] >>> 9;
  assign y[7] = s4_odd[3]  >>> 9;

endmodule
