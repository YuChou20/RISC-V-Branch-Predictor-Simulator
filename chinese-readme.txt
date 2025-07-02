1071536 Project3 assembler readme

在執行程式前請確保資料夾底下包含以下txt檔:
	1.riscv_code.txt:
		此檔案的內容為使用者自行輸入，內容為input data(要predict的risc V code)
		格式為一個在rars上可執行的code，且符合以下條件:

			1.如欲在instruction前加label，則使用label後+冒號+" "(一個空格)+instruction
				Ex:loop: addi x2,x5,16

			2.如每個label單獨佔一行則為label+冒號(後面不可以有任何空格)
				Ex:loop:
				   addi x2,x5,16

			3.每個instruction輸入格式為
				 i.運算子和第一個register中間有空格
				ii.之後都不能有空格(用逗號隔開)
				Ex:addi x2,x5,16 
				   sb x3,0(x5)

			4.不能使用tab!!

			5.每行instruction必須最前面不可以有空格或tab

			6.不支援任何註解(//,#...)

cpp內的全域變數名稱:
	char ctemp[225]:char的temp，在讀檔的時候會使用
	vector<vector<string> >code:儲存risc v code的buffer(下面會詳細說明)
	long reg[33]:存放32個register的值
	map<string,unsigned long>label:儲存每個label的PC(這裡的PC是0,1,2,.....)
	int hbn:history bits有多少bits，在執行前會由使用者輸入給值
	int en:entry的數量，在執行前會由使用者輸入給值

cpp function:
	1.void read_code():將riscv_code.txt檔內的資料一行一行讀入，分成以下幾種情況:
		i.有label沒有instruction:
			把label名和label的PC insert到label(map<string,int>)
		ii.有label有intruction:
			把label名和label的PC insert到label(map<string,int>)，
			把label去掉然後進入到第三種情況(只剩instruction)
		iii.沒有label:
			把instruction拆成一部分一部分(operator,operand)並逐一push_back到code中
			若是register只會儲存register編號
			Ex:addi R3,R5,3  (PC = 2)
			      則code[2][0] = "addi"
				code[2][1] = "3"
				code[2][2] = "5"
				code[2][3] = "3"
			   li R7,15   (PC = 7)
			      則code[7][0] = "li"
				code[7][1] = "7"
				code[7][2] = "15"
		*這樣切割的目的是為了能在執行instruction時不需要重複解析instruction要做什麼事情

	2.void start_predict():開始執行brench預測，對應著使用者輸入的en和hbn
		產生區域變數:
			i.history bits(vector<string> hb)
			ii.en * 2^hbn個2BC(vector<vector<string> > tbc)	//
			iii.misprediction(int* mis)
		接著依據code[i][0]分成兩種cases:
			i.code[i][0] = "beq"
				計算對應到history的entry
				呼叫prediction預測並根據現實taken情況修改history bits和2BC
				預測錯誤就++mis[entry]

			ii.code[i][0] != "beq"
				依據code[i][0]來執行li,add,addi，register和imm則是使用string先強制轉成char再轉換成int來使用
	3.void predict():輸出某個entry的history bits和2BC，並針對branch 來進行修改
	4.stoi():傳入一個string(binary)return一個對應的int

程式的執行程序:
	開始程式前會先把riscv code讀進來做分割，接著會請使用者輸入兩個數字代表history bits要多少個以及要幾個enries
	輸入後開始模擬code，對每一個instruction做出對應的動作，遇到branch則是呼叫predict()來輸出歷史紀錄、預測及現實情況
	注意!每次開始執行程式時，只能針對同一個risc V code 針對不同的history bits和entry數進行預測，如要預測新的code
	     需要修改riscv_code.txt再重新執行檔案