# hw4
4-1:XBee Controlled BB Car
我將parameters(d1,d2,direction)透過keyboard輸入RPC中，direction代表車頭方位，若車庫開口朝南，1代表一開始的車頭朝西，2代表車頭朝南，3代表車頭朝東，parameters經由XBee傳到PC，接著在PC運算
BBcar需要移動的路程以及轉彎幅度。

4-2:Line Following BB Car
我首先利用Python運算OpenMV攝像頭的資料，並取得BBcar需要轉動的角度，接著用UART將角度傳到PC做進一步的運算，得出需要給予servo多少的轉速，使其能follow一條直線，由於我實驗場地偏暗，若該線段為黑色
會不容易萃取特徵值，因此比起黑線我選擇用白線。

4-3:BB Car Position Calibration
這個部分需要利用OpenMV記錄的viewing angle以及PING傳來的距離來判斷車子與AprilTag之間的距離以及角度，因為UART的傳輸速度非常快，因此我選擇不在main function計算距離與驅動BBcar(上一題也是如此)
，而是連兩條thread出去平行運算，根據距離以及viewing angle我得以確定車子該逆時針轉、順時針轉，或是不轉(我將不轉的範圍定在358度~2度之間)，我想了許多方法讓車子移動，最後找到方法能最有效率校正
BBcar的位置。
