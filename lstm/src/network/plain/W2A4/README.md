# W2A4 Weights and Folding description

Weights and Folding description of the W2A4 neural network.

## Utilization reports

Utilization reports using Vivado Design Suite 2017.4:
```
------------------------------------------------------------------------------------------------
| Design Timing Summary
| ---------------------
------------------------------------------------------------------------------------------------

WNS(ns)      TNS(ns)  TNS Failing Endpoints  TNS Total Endpoints      WHS(ns)      THS(ns)  THS Failing Endpoints 
-------      -------  ---------------------  -------------------      -------      -------  ---------------------  
  0.376        0.000                      0                67415        0.010        0.000                      0  


All user specified timing constraints are met.

+----------------------------+-------+-------+-----------+-------+
|          Site Type         |  Used | Fixed | Available | Util% |
+----------------------------+-------+-------+-----------+-------+
| Slice LUTs                 | 26173 |     0 |     53200 | 49.20 |
|   LUT as Logic             | 23123 |     0 |     53200 | 43.46 |
|   LUT as Memory            |  3050 |     0 |     17400 | 17.53 |
|     LUT as Distributed RAM |    10 |     0 |           |       |
|     LUT as Shift Register  |  3040 |     0 |           |       |
| Slice Registers            | 24649 |     0 |    106400 | 23.17 |
|   Register as Flip Flop    | 24649 |     0 |    106400 | 23.17 |
|   Register as Latch        |     0 |     0 |    106400 |  0.00 |
| F7 Muxes                   |    28 |     0 |     26600 |  0.11 |
| F8 Muxes                   |     0 |     0 |     13300 |  0.00 |
+----------------------------+-------+-------+-----------+-------+
+-------------------+------+-------+-----------+-------+
|     Site Type     | Used | Fixed | Available | Util% |
+-------------------+------+-------+-----------+-------+
| Block RAM Tile    | 83.5 |     0 |       140 | 59.64 |
|   RAMB36/FIFO*    |   41 |     0 |       140 | 29.29 |
|     RAMB36E1 only |   41 |       |           |       |
|   RAMB18          |   85 |     0 |       280 | 30.36 |
|     RAMB18E1 only |   85 |       |           |       |
+-------------------+------+-------+-----------+-------+
+----------------+------+-------+-----------+-------+
|    Site Type   | Used | Fixed | Available | Util% |
+----------------+------+-------+-----------+-------+
| DSPs           |    7 |     0 |       220 |  3.18 |
|   DSP48E1 only |    7 |       |           |       |
+----------------+------+-------+-----------+-------+
```

