# MultiProcess-MultiThreads
課程：
作業系統

使用語言：
C/C++

說明：
將檔案內容分成K等份，利用MultiProcess及MultiThread以 BubbleSort及MergeSort進行排序，比較不同個數的Process和Thread在不同資料量(1W~100W)執行速度的差異。
<方法一> N個數字直接做Bubble Sort
<方法二> N個數字切成K份，在一個process中對K份資料分別進行Bubble Sort後再做Merge Sort
<方法三> N個數字切成K份，由K個processes對K份資料分別進行Bubble Sort後再做Merge Sort
<方法四> N個數字切成K份，由K個threads對K份資料分別進行Bubble Sort後再做Merge Sort
