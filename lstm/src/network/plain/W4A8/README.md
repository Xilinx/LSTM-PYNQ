# W4A8 Weights and Folding description

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
  0.725        0.000                      0               104710        0.015        0.000                      0  


All user specified timing constraints are met.

+----------------------------+-------+-------+-----------+-------+
|          Site Type         |  Used | Fixed | Available | Util% |
+----------------------------+-------+-------+-----------+-------+
| Slice LUTs                 | 41895 |     0 |     53200 | 78.75 |
|   LUT as Logic             | 39792 |     0 |     53200 | 74.80 |
|   LUT as Memory            |  2103 |     0 |     17400 | 12.09 |
|     LUT as Distributed RAM |    10 |     0 |           |       |
|     LUT as Shift Register  |  2093 |     0 |           |       |
| Slice Registers            | 41588 |     0 |    106400 | 39.09 |
|   Register as Flip Flop    | 41588 |     0 |    106400 | 39.09 |
|   Register as Latch        |     0 |     0 |    106400 |  0.00 |
| F7 Muxes                   |    35 |     0 |     26600 |  0.13 |
| F8 Muxes                   |     0 |     0 |     13300 |  0.00 |
+----------------------------+-------+-------+-----------+-------+
+-------------------+------+-------+-----------+-------+
|     Site Type     | Used | Fixed | Available | Util% |
+-------------------+------+-------+-----------+-------+
| Block RAM Tile    |  105 |     0 |       140 | 75.00 |
|   RAMB36/FIFO*    |   82 |     0 |       140 | 58.57 |
|     RAMB36E1 only |   82 |       |           |       |
|   RAMB18          |   46 |     0 |       280 | 16.43 |
|     RAMB18E1 only |   46 |       |           |       |
+-------------------+------+-------+-----------+-------+
+----------------+------+-------+-----------+-------+
|    Site Type   | Used | Fixed | Available | Util% |
+----------------+------+-------+-----------+-------+
| DSPs           |    7 |     0 |       220 |  3.18 |
|   DSP48E1 only |    7 |       |           |       |
+----------------+------+-------+-----------+-------+
```

