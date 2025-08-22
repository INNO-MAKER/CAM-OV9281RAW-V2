
Due some data structure modification after kernel v5.10.63 . 
One macro defintion '#define KERNEL_5_15 1' (Lines 50) is added in /inno_mipi_ov9281/inno_mipi_ov9281.c.
If you are using the kernel earlier than (or equal to) v5.10.63, please mask it before compile. 