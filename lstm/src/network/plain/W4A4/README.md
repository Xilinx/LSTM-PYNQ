# W4A4 Weights and Folding description

Weights and Folding description of the W4A4 neural network.

## Utilization reports

Utilization reports using Vivado Design Suite 2017.4:
```
------------------------------------------------------------------------------------------------
| Design Timing Summary
| ---------------------
------------------------------------------------------------------------------------------------

WNS(ns)      TNS(ns)  TNS Failing Endpoints  TNS Total Endpoints      WHS(ns)      THS(ns)  THS Failing Endpoints 
-------      -------  ---------------------  -------------------      -------      -------  ---------------------  
  1.012        0.000                      0                72543        0.008        0.000                      0  


All user specified timing constraints are met.

+----------------------------+-------+-------+-----------+-------+
|          Site Type         |  Used | Fixed | Available | Util% |
+----------------------------+-------+-------+-----------+-------+
| Slice LUTs                 | 38260 |     0 |     53200 | 71.92 |
|   LUT as Logic             | 35064 |     0 |     53200 | 65.91 |
|   LUT as Memory            |  3196 |     0 |     17400 | 18.37 |
|     LUT as Distributed RAM |    10 |     0 |           |       |
|     LUT as Shift Register  |  3186 |     0 |           |       |
| Slice Registers            | 26618 |     0 |    106400 | 25.02 |
|   Register as Flip Flop    | 26618 |     0 |    106400 | 25.02 |
|   Register as Latch        |     0 |     0 |    106400 |  0.00 |
| F7 Muxes                   |   675 |     0 |     26600 |  2.54 |
| F8 Muxes                   |     0 |     0 |     13300 |  0.00 |
+----------------------------+-------+-------+-----------+-------+
+-------------------+-------+-------+-----------+-------+
|     Site Type     |  Used | Fixed | Available | Util% |
+-------------------+-------+-------+-----------+-------+
| Block RAM Tile    | 103.5 |     0 |       140 | 73.93 |
|   RAMB36/FIFO*    |    41 |     0 |       140 | 29.29 |
|     RAMB36E1 only |    41 |       |           |       |
|   RAMB18          |   125 |     0 |       280 | 44.64 |
|     RAMB18E1 only |   125 |       |           |       |
+-------------------+-------+-------+-----------+-------+
+----------------+------+-------+-----------+-------+
|    Site Type   | Used | Fixed | Available | Util% |
+----------------+------+-------+-----------+-------+
| DSPs           |    7 |     0 |       220 |  3.18 |
|   DSP48E1 only |    7 |       |           |       |
+----------------+------+-------+-----------+-------+
```

