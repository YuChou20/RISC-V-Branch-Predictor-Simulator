#include<iostream>
#include<vector>
#include<fstream>
#include<vector>
#include<map>
#include<cmath>
#include<bitset>
using namespace std;
char ctemp[255];
long reg[33];
string stemp;
vector<vector<string> > code;
map<string, unsigned long>label;
int en = 0, hbn = 0;	//en = number of entries   , hbn = number of history bits

long stoi(string sn) {
	bitset<32> num(sn);
	return num.to_ulong();
}

void readcode() {
	ifstream infile("riscv_code.txt");
	while (infile.getline(ctemp, 255)) {
		stemp = ctemp;
		string::size_type pos = stemp.find(":");
		if (pos != stemp.npos) {
			label.insert(make_pair(stemp.substr(0, pos), code.size()));			//add label and save the NO of the instruction
			if (stemp.size() != pos + 1) {
				stemp = stemp.substr(pos + 1, stemp.size());
			}
			else {
				stemp = "";
			}
		}
		for (int i = 0; i < stemp.size(); ++i) {
			if (stemp[i] != ' ' && stemp[i] != '\t') {
				stemp = stemp.substr(i, stemp.size());
				break;
			}
		}
		if (stemp != "") {
			code.push_back(vector<string>());
			string stp = "";
			for (int i = 0; i < stemp.size(); ++i) {
				if (stemp[i] != ' ' && stemp[i] != ',') {
					if (stemp[i] != 'R' && stemp[i] != 'x') {
						stp += stemp[i];
					}
				}
				else{
					code[code.size()-1].push_back(stp);
					stp = "";
				}
			}
			code[code.size() - 1].push_back(stp);
		}
	}
	infile.close();
}

void predict(vector<string>& tbc,string hb , bool istaken,int& mis) {
	int ind = stoi(hb);
	cout << "(" << hb;
	for (int i = 0; i < pow(2,hbn); ++i) {
		cout << ", " << tbc[i];
	}
	cout << ") ";
	if (tbc[ind] == "SN") {
		cout << "N ";
		if (istaken) {
			++mis;
			tbc[ind] = "WN";
		}
	}
	else if (tbc[ind] == "WN") {
		cout << "N ";
		if (istaken) {
			++mis;
			tbc[ind] = "WT";
		}
		else {
			tbc[ind] = "SN";
		}
	}
	else if (tbc[ind] == "WT") {
		cout << "T ";
		if (istaken) {
			tbc[ind] = "ST";
		}
		else {
			++mis;
			tbc[ind] = "WN";
		}
	}
	else if (tbc[ind] == "ST") {
		cout << "T ";
		if (!istaken) {
			++mis;
			tbc[ind] = "WT";
		}
	}
	if (istaken) {
		cout << "T";
	}
	else {
		cout << "N";
	}
	cout << "	misprediction:  " << mis << endl;
}

void start_predict(){
	string tmp = "";
	for (int i = 0; i < hbn; ++i) {
		tmp += "0";
	}
	vector<string> hb(en,tmp);
	vector<vector<string> > tbc;
	int* mis = new int[en]();
	for (int i = 0; i < en; ++i) {
		tbc.push_back(vector<string>(pow(2, hbn),"SN"));
	}
	for (int i = 0; i < code.size();) {
		reg[0] = 0;
		if (code[i][0] == "beq") {
			int ind = i % en;
			bool istaken = reg[atoi(code[i][1].c_str())] == reg[atoi(code[i][2].c_str())];
			cout << "entry:  " << ind << "	";
			cout << code[i][0] << " R" << code[i][1] << ",R" << code[i][2] << "," << code[i][3] << endl;
			predict(tbc[ind], hb[ind], istaken, mis[ind]);
			if (istaken) {
				i = label[code[i][3]];
				hb[ind] = hb[ind].substr(1, hb[ind].size()) + "1";
			}
			else {
				++i;
				hb[ind] = hb[ind].substr(1, hb[ind].size()) + "0";
			}
			cout << endl << endl;
		}
		else {
			if (code[i][0] == "li") {
				reg[atoi(code[i][1].c_str())] = atoi(code[i][2].c_str());
			}
			else if (code[i][0] == "addi") {
				reg[atoi(code[i][1].c_str())] = reg[atoi(code[i][2].c_str())] + atoi(code[i][3].c_str());
			}
			else if (code[i][0] == "add") {
				reg[atoi(code[i][1].c_str())] = reg[atoi(code[i][2].c_str())] + reg[atoi(code[i][3].c_str())];
			}
			++i;
		}
	}
}

int main() {
	readcode();
	while (true) {
		cout << "How many bits(history bits)?\n";
		cin >> hbn;
		if (hbn == 0) {
			break;
		}
		cout << "How many entries?\n";
		cin >> en;
		start_predict();
	}
}