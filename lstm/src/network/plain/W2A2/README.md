# W2A2 Weights and Folding description

Weights and Folding description of the W2A2 neural network.

## Utilization reports

Utilization reports using Vivado Design Suite 2017.4:
```
------------------------------------------------------------------------------------------------
| Design Timing Summary
| ---------------------
------------------------------------------------------------------------------------------------

WNS(ns)      TNS(ns)  TNS Failing Endpoints  TNS Total Endpoints      WHS(ns)      THS(ns)  THS Failing Endpoints 
-------      -------  ---------------------  -------------------      -------      -------  ---------------------  
  0.920        0.000                      0               124468        0.022        0.000                      0  


All user specified timing constraints are met.

+----------------------------+-------+-------+-----------+-------+
|          Site Type         |  Used | Fixed | Available | Util% |
+----------------------------+-------+-------+-----------+-------+
| Slice LUTs                 | 26423 |     0 |     53200 | 49.67 |
|   LUT as Logic             | 20011 |     0 |     53200 | 37.61 |
|   LUT as Memory            |  6412 |     0 |     17400 | 36.85 |
|     LUT as Distributed RAM |    10 |     0 |           |       |
|     LUT as Shift Register  |  6402 |     0 |           |       |
| Slice Registers            | 39703 |     0 |    106400 | 37.31 |
|   Register as Flip Flop    | 39703 |     0 |    106400 | 37.31 |
|   Register as Latch        |     0 |     0 |    106400 |  0.00 |
| F7 Muxes                   |   587 |     0 |     26600 |  2.21 |
| F8 Muxes                   |   224 |     0 |     13300 |  1.68 |
+----------------------------+-------+-------+-----------+-------+
+-------------------+-------+-------+-----------+-------+
|     Site Type     |  Used | Fixed | Available | Util% |
+-------------------+-------+-------+-----------+-------+
| Block RAM Tile    | 119.5 |     0 |       140 | 85.36 |
|   RAMB36/FIFO*    |    34 |     0 |       140 | 24.29 |
|     RAMB36E1 only |    34 |       |           |       |
|   RAMB18          |   171 |     0 |       280 | 61.07 |
|     RAMB18E1 only |   171 |       |           |       |
+-------------------+-------+-------+-----------+-------+
+----------------+------+-------+-----------+-------+
|    Site Type   | Used | Fixed | Available | Util% |
+----------------+------+-------+-----------+-------+
| DSPs           |  180 |     0 |       220 | 81.82 |
|   DSP48E1 only |  180 |       |           |       |
+----------------+------+-------+-----------+-------+
```

