#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
#define pb push_back
#define START 268435456
vector<pair<string,int>> codeinit;
ll PC = 0;
string datamemory[4000]; // Initial Size of Data Memory is Fixed 4000 Bytes

struct code_code
{
	string arg1;
	ll arg2, arg3, arg4;
	int indexs;
};

vector<code_code> code;

typedef struct 
{
	string s;
	int index;
}lab;

vector<lab> Label;

struct datafile
{
    string name;
    string type;
    vector<string> value;
};

vector<datafile> stored;

struct seg
{
    string name;
    ll position;
};

vector<seg>datalabel;

class formate_of_ins
{
    public:
        string format;
        string type;
        string opcode;
        string fun3;
        string fun7;
};

vector<formate_of_ins> Format;

void storing_formates()
{
    ifstream file;
    string str;
    file.open("my_format.txt");
    if(file.is_open())
    {
        while(!file.eof())
        {
            getline(file, str);
            istringstream ss(str);
            string word;
            int i = 0;
            formate_of_ins fr;
            while (ss >> word) 
            {
                if(i==0)
                {
                    fr.format = word;
                }
                else if(i==1)
                {
                    fr.type = word;
                }
                else if(i==2)
                {
                    fr.opcode = word;
                }
                else if(i==3)
                {
                    fr.fun3 = word;
                }
                else if(i==4)
                {
                    fr.fun7 = word;
                }
                i++;
            }
            Format.pb(fr);
        }
        
    }
    file.close();
}

//To get 2's Complement Representation for Immediate Values
ll getinver(ll imme,int bit)
{
	ll imcopy=-imme;
	vector<int>bb;
	imme=-imme;
	for(int i=0;i<bit;i++)
	{
		bb.push_back(imme%2);
		imme/=2;
	}
	reverse(bb.begin(),bb.end());
	for(int i=0;i<bit;i++)
	{
		if(bb[i])
			bb[i]=0;
		else
			bb[i]=1;
	}
	int j=bit-1;
	while(1)
	{
		if(bb[j]==0)
		{
			bb[j]=1;
			break;
		}
		else{
			bb[j]=0;
		}
		j--;
	}
	ll num=0;
	ll mul=1;
	for(int i=bit-1;i>=0;i--)
	{
		num += bb[i]*mul;
		mul*=2;
	}
	return num;
}

//To convert a number in string format to its Hexadecimal
string convert(string s,int len)
{
    int length = s.size();
    int flag = 1;
    for(int i=0;i<length;i++)
    {
        if(s[i]=='x' || s[i]=='X')
        {
            flag = 0;
            break;
        }
    }
    if(flag)
    {
        ll num = 0;
        ll mul = 1;
        int last = 0;
        int flag1 = 0;
        if(s[0]=='-')		flag1 = 1;
        if(flag1)		last = 1;
        for(int i = length-1;i>=last;i--)
        {
            num += (s[i]-48)*mul;
            mul*=10;
        }

        if(flag1)
        {
        	num = getinver(num,len*4);
        }

        string ans;
        for(int i=0;i<len;i++)
        {
            ll rem = num%16;
            if(rem<=9)
            {
                ans += (rem+48);
            }
            else
            {
                ans += (rem-10+65);
            }
            num/=16;
        }
        reverse(ans.begin(),ans.end());
        return ans;
    }

    else
    {
        ll x; 
        stringstream ss; 
        ss << std::hex << s; 
        ss >> x; 
        ll num = x;
        if(num<0)
        {
        	num = getinver(num,len*4);
        }
        string ans;
        for(int i=0;i<len;i++)
        {
            ll rem = num%16;
            if(rem<=9)
            {
                ans += (rem+48);
            }
            else
            {
                ans += (rem-10+65);
            }
            num/=16;
        }
        reverse(ans.begin(),ans.end());
        return ans;
    }
}

string split(string str, char del = ',')
{
    // declaring temp string to store the curr "word" upto del
      string temp = "";
   
      for(int i=0; i<(int)str.size(); i++){
        // If cur char is not del, then append it to the cur "word", otherwise
          // you have completed the word, print it, and start a new word.
         if(str[i] != del){
            temp += str[i];
        }
          else{
              temp = temp + " ";
        }
    }

    return temp;
}

ll stringToInteger(const string& str) 
{
    istringstream iss(str);
    ll result;
    // Check if the string starts with "0x" for hexadecimal
    if (str.substr(0, 2) == "0x") {
        iss >> hex >> result;
    } else {
        iss >> dec >> result;
    }
    return result;
}

void read_data()
{
    ifstream file;
    string word;
	regex reg_exp_data("[, ]*[a-zA-Z0-9_\\.]+[ ]*:[, ]*\\.(word|half|byte|dword)[, ]+(([0-9]+)|(0x([0-9a-fA-F]{1,16})))([ ,]*)([, ](([0-9]+)|(0x[0-9A-Fa-f]{1,16}))[, ]*)*(#(.)*)?");
	regex reg_exp_data2("[, ]*[a-zA-Z0-9_\\.]+[ ]*:[, ]*\\.(asciiz)[, ]*\"(.)*\"[ ,]*(#(.)*)?");
	file.open("test.asm");
    int start = 0;
	int line_no = 0;
	if(file.is_open())
    {
		while(!file.eof())
        {
			line_no++;
			getline(file, word);
			// for ignoring any comment line in data segment
			string s1 = word,s2;
			istringstream ss(s1);
			ss >> s2;
			if(s2[0] == '#')
			{
				continue;
			}
			// for ignoring any empty line
			if(word.size() == 0)
			{
				continue;
			}
			if(start > 1)
            {
                continue;
            }
            else if(start == 1)
            {
				string ttword = word;
				ttword = ttword.substr(0, ttword.find("#"));
				istringstream sword(ttword);
				string stemp;
				sword >> stemp;
				if(stemp == ".text")         //data part ends
                    start = 2;
                else
                {
                    if (regex_match(word, reg_exp_data))
					{
						datafile temp;

						string word1 = split(word);
    					char* str = &word1[0];
    					char *token = strtok(str, " ");
						int size = 0;
						while (token != NULL)
    					{
							size++;
							token = strtok(NULL, " ");
    					}
						string word2 = split(word);
						char *str2 = &word2[0];
						char *token2 = strtok(str2, " ");
						int i = 0;
						int flag = 0;
						while(token2 != NULL)
						{
							string tempW;
							tempW = token2;
							int len = tempW.size();
							if(i==0)
							{
								if(tempW[len-1] == ':')
								{
									flag = 1;
									tempW.erase(len-1);
									temp.name = tempW;
								}
								else
								{
									string eg = "";
									int idx;
									for (idx = 0; idx < len; idx++)
									{
										if(tempW[idx] == ':')
										{
											flag = 2;
											break;
										}
										else
										{
											eg = eg + tempW[idx];
										}
									}
									temp.name = eg;
									if(idx == len)
									{
										flag = 3;
									}
									eg = "";
									if(flag == 2)
									{
										for (idx = idx + 2; idx < len; idx++)
										{
											eg = eg + tempW[idx];
										}
										temp.type = eg;
									}
								}
							}
							else
							{
								int fla;
								if(flag == 1)
								{
									if(i==1)
									{
										tempW.erase(tempW.begin());
                        				temp.type = tempW;
									}
									else
									{
										string ex = "";
										for (int j = 0; j < len; j++)
										{
											if(tempW[j]=='#')
											{
												flag = 6;
												break;
											}
											else
											{
												ex = ex + tempW[j];
											}
										}
										if(ex.size()!=0)
										{
											temp.value.pb(ex);
										}
										if(flag == 6)
										{
											break;
										}
									}
								}
								else if(flag == 2)
								{
									string ex = "";
									for (int j = 0; j < len; j++)
									{
										if(tempW[j]=='#')
										{
											flag = 6;
											break;
										}
										else
										{
											ex = ex + tempW[j];
										}
									}
									if(ex.size()!=0)
									{
										temp.value.pb(ex);
									}
									if(flag == 6)
									{
										break;
									}
								}
								else if(flag == 3)
								{
									if(i==1)
									{
										if(len == 1)
										{											
											fla = 4;
										}
										else
										{
											fla = 5;
										}
										if(fla == 5)
										{
											tempW.erase(tempW.begin());
											tempW.erase(tempW.begin());
                        					temp.type = tempW;
										}
									}
									else if(i==2)
									{
										if(fla == 5)
										{
											string ex = "";
											for (int j = 0; j < len; j++)
											{
												if(tempW[j]=='#')
												{
													flag = 6;
													break;
												}
												else
												{
													ex = ex + tempW[j];
												}
											}
											if(ex.size()!=0)
											{
												temp.value.pb(ex);
											}
											if(flag == 6)
											{
												break;
											}
										}
										else if(fla == 4)
										{
											tempW.erase(tempW.begin());
                        					temp.type = tempW;
										}
									}
									else
									{
										string ex = "";
										for (int j = 0; j < len; j++)
										{
											if(tempW[j]=='#')
											{
												flag = 6;
												break;
											}
											else
											{
												ex = ex + tempW[j];
											}
										}
										if(ex.size()!=0)
										{
											temp.value.pb(ex);
										}
										if(flag == 6)
										{
											break;
										}
									}

								}
							}
							i++;
							token2 = strtok(NULL, " ");
						}
						for (int k = 0; k < stored.size();k++)
						{
							if(temp.name == stored[k].name)
							{
								cout << "error in line no : " << line_no << " variable name is already defined" << endl;
								exit(0);
							}
						}
						stored.pb(temp);
					}
					else if(regex_match(word, reg_exp_data2))
					{
						datafile temp;
						word = word.substr(0, word.find("#"));
						int i = 0;
						for (i ; i < word.size(); i++)
						{
							if(word[i] == ' ' || word[i] == ',')
							{
								continue;
							}
							else
							{
								break;
							}
						}
						string tname = "";
						for(i ; i<word.size() ; i++)
						{
							if(word[i]==' ' || word[i] == ':')
							{
								break;
							}
							else
							{
								tname = tname + word[i];
							}
						}
						temp.name = tname;
						for (i; i < word.size(); i++)
						{
							if(word[i] == ':')
							{
								break;
							}
							else
							{
								continue;
							}
						}
						i++;
						for (i; i<word.size() ; i++)
						{
							if(word[i] == '.')
							{
								break;
							}
							else
							{
								continue;
							}
						}
						i++;
						string ttype = "";
						for (i; i < word.size(); i++)
						{
							if(word[i]==' ' || word[i] == ',' || word[i] == '"')
							{
								break;
							}
							else
							{
								ttype = ttype + word[i];
							}
						}
						temp.type = ttype;
						for (i; i < word.size(); i++)
						{
							if(word[i]=='"')
							{
								break;
							}
						}
						i++;
						string tvalue;
						for (i; i < word.size();i++)
						{
							if(word[i]=='"')
							{
								break;
							}
							else
							{
								tvalue = tvalue + word[i];
							}
						}
						temp.value.pb(tvalue);
						for (int k = 0; k < stored.size();k++)
						{
							if(temp.name == stored[k].name)
							{
								cout << "error in line no : " << line_no << " variable name is already defined" << endl;
								exit(0);
							}
						}
						stored.pb(temp);
					}
					else
					{
						cout << "Error in line no : " << line_no << endl;
						exit(0);
					}
                }
            }
            else if(start == 0)
            {
				string ttword = word;
				ttword = ttword.substr(0, ttword.find("#"));
				istringstream sword(ttword);
				string stemp;
				sword >> stemp;
                if(stemp == ".data")  
				{
                    start = 1;
				}
            }
        }
    }
    file.close();
    int pos = 0;
    for(int i = 0; i < stored.size(); i++)
    {

        seg temp;   temp.name = stored[i].name;    temp.position = pos + START;  
        datalabel.push_back(temp);
        if(stored[i].type == "byte")
        {
            for(int j = 0; j < stored[i].value.size(); j++)
            {
				ll myInteger = stringToInteger(stored[i].value[j]);
				if(stored[i].value[j][1] != 'x')
				{
					if(myInteger<-pow(2,7) || myInteger>=pow(2,7))
					{
						cout << "error in data segment in byte part" << endl;
						exit(0);
					}
				}
				else
				{
					if(myInteger>=pow(2,8))
					{
						cout << "error in data segment in byte part" << endl;
						exit(0);
					}
				}
                string s = convert(stored[i].value[j],2);
                datamemory[pos++] = s;
            }
        }
        else if(stored[i].type == "word")
        {
            for(int j = 0; j < stored[i].value.size(); j++)
            {
				ll myInteger = stringToInteger(stored[i].value[j]);
				if(stored[i].value[j][1] != 'x')
				{
					if(myInteger<-pow(2,31) || myInteger>=pow(2,31))
					{
						cout << "error in data segment in word part" << endl;
						exit(0);
					}
				}
				else
				{
					if(myInteger>=pow(2,32))
					{
						cout << "error in data segment in word part" << endl;
						exit(0);
					}
				}
                string s = convert(stored[i].value[j],8);
                string temp;    temp+=s[6]; temp+=s[7];
                datamemory[pos++] = temp; temp.clear();
                temp+=s[4]; temp+=s[5];
                datamemory[pos++] = temp; temp.clear();
                temp+=s[2]; temp+=s[3];
                datamemory[pos++] = temp; temp.clear();
                temp+=s[0]; temp+=s[1];
                datamemory[pos++] = temp; temp.clear();
            }
        }
		else if(stored[i].type == "dword")
        {
            for(int j = 0; j < stored[i].value.size(); j++)
            {
				ll myInteger = stringToInteger(stored[i].value[j]);
				if(stored[i].value[j][1] != 'x')
				{
					if(myInteger<-pow(2,63) || myInteger>=pow(2,63))
					{
						cout << "error in data segment in double word part" << endl;
						exit(0);
					}
				}
				else
				{
					if(myInteger>=pow(2,64))
					{
						cout << "error in data segment in double word part" << endl;
						exit(0);
					}
				}
				string s = convert(stored[i].value[j],16);
				string temp;    temp+=s[14]; temp+=s[15];
                datamemory[pos++] = temp; temp.clear();
                temp+=s[12]; temp+=s[13];
                datamemory[pos++] = temp; temp.clear();
                temp+=s[10]; temp+=s[11];
                datamemory[pos++] = temp; temp.clear();
                temp+=s[8]; temp+=s[9];
                datamemory[pos++] = temp; temp.clear();
				temp+=s[6]; temp+=s[7];
                datamemory[pos++] = temp; temp.clear();
                temp+=s[4]; temp+=s[5];
                datamemory[pos++] = temp; temp.clear();
                temp+=s[2]; temp+=s[3];
                datamemory[pos++] = temp; temp.clear();
                temp+=s[0]; temp+=s[1];
                datamemory[pos++] = temp; temp.clear();
            }
        }
        else if(stored[i].type == "half")
        {
             for(int j = 0; j < stored[i].value.size(); j++)
            {
				ll myInteger = stringToInteger(stored[i].value[j]);
				if(stored[i].value[j][1] != 'x')
				{
					if(myInteger<-pow(2,15) || myInteger>=pow(2,15))
					{
						cout << "error in data segment in half word part" << endl;
						exit(0);
					}
				}
				else
				{
					if(myInteger>=pow(2,16))
					{
						cout << "error in data segment in half word part" << endl;
						exit(0);
					}
				}
                string s = convert(stored[i].value[j],4);
                string temp;    
                temp+=s[2]; temp+=s[3];
                datamemory[pos++] = temp; temp.clear();
                temp+=s[0]; temp+=s[1];
                datamemory[pos++] = temp; temp.clear();
            }
        }
		else if(stored[i].type == "asciiz")
		{
           	string s = stored[i].value[0];
           	int length=s.size();
		   	for(int k=0;k<length;k++)
		   	{
				int at = (int)s[k];
				string myString = to_string(at);
				string ss = convert(myString,2);
           	 	datamemory[pos++] = ss;
		   	}
			pos++;
		}
    }
}

void read_inst()	
{
    ifstream myFile;
	myFile.open("test.asm");
	string line;
	int flag = 0;
	int start = 0;
	int line_no = 0;
	int count = 0;
	while(getline(myFile,line))
	{
		line_no++;
		line = line.substr(0, line.find("#"));

		line = split(line);
    	istringstream ss(line);
		string word;
		ss >> word;
		if(word.size() == 0)
		{
			continue;
		}

		if(word==".data")
			flag = 1;
		if(word==".text")
		{
			flag = 0;
			continue;
		}
		if(flag!=1)
		{
			lab temp;
			regex reg_exp1("[ ]*[a-zA-Z0-9_\\.]+[ ]*:[ ]*");// (([0-9]+)\\((x([0-9]|1[0-9]|2[0-9]|3[0-1]))\\))
			regex reg_exp2("[ ]*[a-z]{2,5}[ ]+(x([0-9]|1[0-9]|2[0-9]|3[0-1]))[ ]+((x([0-9]|1[0-9]|2[0-9]|3[0-1]))|(((-)?[0-9]+)|(0x[A-Fa-f0-9]{1,8}))|([a-zA-Z0-9_\\.]+)|(((-)?[0-9]+)[ ]*\\((x([0-9]|1[0-9]|2[0-9]|3[0-1]))\\)))[ ]*(((x([0-9]|1[0-9]|2[0-9]|3[0-1]))|(((-)?[0-9]+)|(0x[A-Fa-f0-9]{1,8}))|([a-zA-Z0-9_\\.]+))[ ]*)?");//(x([0-9]|1[0-9]|2[0-9]|3[0-1]))
			regex reg_exp3("[ ]*[a-zA-Z0-9_\\.]+[ ]*:[ ]*[a-z]{2,5}[ ]+(x([0-9]|1[0-9]|2[0-9]|3[0-1]))[ ]+((x([0-9]|1[0-9]|2[0-9]|3[0-1]))|(((-)?[0-9]+)|(0x[A-Fa-f0-9]{1,8}))|([a-zA-Z0-9_\\.]+)|(((-)?[0-9]+)[ ]*\\((x([0-9]|1[0-9]|2[0-9]|3[0-1]))\\)))[ ]*(((x([0-9]|1[0-9]|2[0-9]|3[0-1]))|(((-)?[0-9]+)|(0x[A-Fa-f0-9]{1,8}))|([a-zA-Z0-9_\\.]+))[ ]*)?");
			if(regex_match(line, reg_exp1))
			{
				line = line.substr(0, line.find(":"));
				istringstream ss(line);
				string word;
				ss >> word;
				count++;
				temp.index = count;
				count--;
				temp.s = word;
				for (int k = 0; k < Label.size(); k++)
				{
					if(Label[k].s == word)
					{
						cout << "error in line no : " << line_no <<" Label is already declared"<< endl;
						exit(0);
					}
				}
				for (int k = 0; k < stored.size(); k++)
				{
					if(stored[k].name == word)
					{
						cout << "error in line no : " << line_no <<" Label is already declared in data segment"<< endl;
						exit(0);
					}
				}
				Label.pb(temp);
			}
			else if(regex_match(line, reg_exp2))
			{
				count++;
				pair<string, int> tem;
				tem.first = line;
				tem.second = line_no;
				codeinit.pb(tem);
			}
			else if(regex_match(line, reg_exp3))
			{
				string temLabel = line.substr(0, line.find(":"));
				istringstream sss(temLabel);
				string wordt;
				sss >> wordt;
				count++;
				temp.index = count;
				temp.s = wordt;
				Label.pb(temp);

				line = line.substr(line.find(":")+1,line.size());
				pair<string, int> tem;
				tem.first = line;
				tem.second = line_no;
				codeinit.pb(tem);
			}
			else
			{
				cout << "error in line no : " << line_no << endl;
				exit(0);
			}
		}
	}
}

ll hex_to_decimal_for_U_format(string hexString) 
{
    stringstream ss;
	ss << hex << hexString;
    ll decimalValue;
    ss >> decimalValue;
    return decimalValue;
}

void process()
{
	int idx = 0;
	while(idx<codeinit.size())
	{
		string line = codeinit[idx].first;
		istringstream ss(line);
		string word;
		ss >> word;
		int flag = 0;
		int index;
		code_code temp;
		for (int i = 0; i < Format.size(); i++)
		{
			if(Format[i].type == word)
			{
				flag = 1;
				index = i;
				temp.arg1 = word;
				temp.indexs = i;
				break;
			}
		}
		if(flag == 0)
		{
			cout << "error in line no : " << codeinit[idx].second << endl;
			exit(0);
		}
		if(Format[index].format == "R")
		{
			int i = 1;
			while(ss >> word)
			{
				if(i==4)
				{
					cout << "error in line no : " << codeinit[idx].second << endl;
					exit(0);
				}
				else
				{
					if(word[0] != 'x')
        			{
        			    cout << "error in line no : " << codeinit[idx].second << endl;
						exit(0);
        			}
					word.erase(word.begin());
        			ll ar = 0;
        			for (int j = 0; j < word.size(); j++)
        			{
						if((word[j]-48)<0 || (word[j]-48)>9)
						{
							cout << "error in line no : " << codeinit[idx].second << endl;
							exit(0);
						}
        			    ar = 10 * ar + (word[j]-48) ;
        			}
					if(ar<0 || ar>31)
					{
						cout << "error in line no : " << codeinit[idx].second << endl;
						exit(0);
					}
					if(i==1)
					{
						temp.arg2 = ar;
					}
					else if(i==2)
					{
						temp.arg3 = ar;
					}
					else if(i==3)
					{
						temp.arg4 = ar;
					}
				}
				i++;
			}
		}
		else if(Format[index].format == "I")
		{
			if(temp.arg1 == "jalr")
			{
				int i = 1;
				while(ss >> word)
				{
					if(i==4)
					{
						cout << "error in line no : " << codeinit[idx].second << endl;
						exit(0);
					}
					else if(i==1 || i==2)
					{
						if(word[0] != 'x')
        				{
        				    cout << "error in line no : " << codeinit[idx].second << endl;
							exit(0);
        				}
						word.erase(word.begin());
        				ll ar = 0;
        				for (int j = 0; j < word.size(); j++)
        				{
							if((word[j]-48)<0 || (word[j]-48)>9)
							{
								cout << "error in line no : " << codeinit[idx].second << endl;
								exit(0);
							}
        				    ar = 10 * ar + (word[j]-48) ;
        				}
						if(ar<0 || ar>31)
						{
							cout << "error in line no : " << codeinit[idx].second << endl;
							exit(0);
						}
						if(i==1)
						{
							temp.arg2 = ar;
						}
						else if(i==2)
						{
							temp.arg3 = ar;
						}	
					}
					else if(i==3)
					{
						int lab_flag = 0;
						int lab_index;
						for (int j = 0; j < Label.size(); j++)
						{
							if(word == Label[j].s)
							{
								lab_index = j;
								lab_flag = 1;
								temp.arg4 = (Label[lab_index].index - 1) * 4;
								break;
							}
						}
						if(lab_flag == 0)
						{
							int arg_flag = 1;
							char minus = '+';
							if(word[0]=='-')
							{
								minus = word[0];
								word.erase(word.begin());
							}
							ll ar = 0;
        					for (int j = 0; j < word.size(); j++)
        					{
								if((word[j]-48)<0 || (word[j]-48)>9)
								{
									cout << "error in line no : " << codeinit[idx].second << endl;
									exit(0);
								}
        					    ar = 10 * ar + (word[j]-48) ;
        					}
							if(minus == '-')
							{
								ar = -1 * ar;
							}
							if(ar<-pow(2,11) || ar>=pow(2,11))
							{
								cout << "error in line no : " << codeinit[idx].second <<" Immidiate is out of Range"<< endl;
								exit(0);
							}
							if(ar%4 != 0)
							{
								cout << "error in line no : " << codeinit[idx].second << endl;
								exit(0);
							}
							temp.arg4 = ar;
							// cout << "error in line no : " << codeinit[idx].second << endl;
							// exit(0);
						}	
					}
					i++;
				}
			}
			else if(temp.arg1 == "lb" || temp.arg1 == "lh" || temp.arg1 == "lw" || temp.arg1 == "ld")
			{
				int arg_flag = 0;
				int i = 1;
				while(ss >> word)
				{
					if(i==4)
					{
						cout << "error in line no : " << codeinit[idx].second << endl;
						exit(0);
					}
					else if(i==1)
					{
						word.erase(word.begin());
        				ll ar = 0;
        				for (int j = 0; j < word.size(); j++)
        				{
        				    ar = 10 * ar + (word[j]-48) ;
        				}
						temp.arg2 = ar;
					}
					else if(i==2)
					{
						regex exp1("[-]?[0-9]+");
						regex exp2("[-]?[0-9]+\\(x([0-9]|1[0-9]|2[0-9]|3[0-1])\\)");
						if(regex_match(word,exp1))
						{
							arg_flag = 1;
							char minus = '+';
							if(word[0]=='-')
							{
								minus = word[0];
								word.erase(word.begin());
							}
							ll ar = 0;
        					for (int j = 0; j < word.size(); j++)
        					{
								if((word[j]-48)<0 || (word[j]-48)>9)
								{
									cout << "error in line no : " << codeinit[idx].second << endl;
									exit(0);
								}
        					    ar = 10 * ar + (word[j]-48) ;
        					}
							if(minus == '-')
							{
								ar = -1 * ar;
							}
							if(ar<-pow(2,11) || ar>=pow(2,11))
							{
								cout << "error in line no : " << codeinit[idx].second <<" Immidiate is out of Range"<< endl;
								exit(0);
							}
							temp.arg4 = ar;
						}
						else if(regex_match(word,exp2))
						{
							char minus = '+';
							int s_index;
							if(word[0]=='-')
							{
								minus = word[0];
								word.erase(word.begin());
							}
							ll ar = 0;
        					for (int j = 0; j < word.size(); j++)
        					{
								if(word[j] == '(')
								{
									s_index = j;
									break;
								}
								if((word[j]-48)<0 || (word[j]-48)>9)
								{
									cout << "error in line no : " << codeinit[idx].second << endl;
									exit(0);
								}
        					    ar = 10 * ar + (word[j]-48) ;
        					}
							if(minus == '-')
							{
								ar = -1 * ar;
							}
							if(ar<-pow(2,11) || ar>=pow(2,11))
							{
								cout << "error in line no : " << codeinit[idx].second <<" Immidiate is out of Range"<< endl;
								exit(0);
							}
							temp.arg4 = ar;
							//.....
							word = word.substr(word.find("(")+1,word.size() - word.find("(") - 2);
							if(word[0] != 'x')
        					{
        					    cout << "error in line no : " << codeinit[idx].second << endl;
								exit(0);
        					}
							word.erase(word.begin());
        					ar = 0;
        					for (int j = 0; j < word.size(); j++)
        					{
								if((word[j]-48)<0 || (word[j]-48)>9)
								{
									cout << "error in line no : " << codeinit[idx].second << endl;
									exit(0);
								}
        					    ar = 10 * ar + (word[j]-48) ;
        					}
							if(ar<0 || ar>31)
							{
								cout << "error in line no : " << codeinit[idx].second << endl;
								exit(0);
							}
							temp.arg3 = ar;
						}
						else
						{
							cout << "error in line no : " << codeinit[idx].second << endl;
							exit(0);
						}
					}
					else if(i==3)
					{
						if(flag == 0)
						{
							cout << "error in line no : " << codeinit[idx].second << endl;
							exit(0);
						}
						if(word[0] == '(')
						{
							if(word[word.size()-1] != ')')
							{
								cout << "error in line no : " << codeinit[idx].second << endl;
								exit(0);
							}
							word.erase(word.begin());
							word.erase(word.end()-1);
						}
						if(word[0] != 'x')
        				{
        				    cout << "error in line no : " << codeinit[idx].second << endl;
							exit(0);
        				}
						word.erase(word.begin());
        				ll ar = 0;
        				for (int j = 0; j < word.size(); j++)
        				{
							if((word[j]-48)<0 || (word[j]-48)>9)
							{
								cout << "error in line no : " << codeinit[idx].second << endl;
								exit(0);
							}
        				    ar = 10 * ar + (word[j]-48) ;
        				}
						if(ar<0 || ar>31)
						{
							cout << "error in line no : " << codeinit[idx].second << endl;
							exit(0);
						}
						temp.arg3 = ar;
					}
					i++;
				}
			}
			else
			{
				int i = 1;
				while(ss >> word)
				{
					if(i==4)
					{
						cout << "error in line no : " << codeinit[idx].second << endl;
						exit(0);
					}
					else if(i==1 || i==2)
					{
						if(word[0] != 'x')
        				{
        				    cout << "error in line no : " << codeinit[idx].second << endl;
							exit(0);
        				}
						word.erase(word.begin());
        				ll ar = 0;
        				for (int j = 0; j < word.size(); j++)
        				{
							if((word[j]-48)<0 || (word[j]-48)>9)
							{
								cout << "error in line no : " << codeinit[idx].second << endl;
								exit(0);
							}
        				    ar = 10 * ar + (word[j]-48) ;
        				}
						if(ar<0 || ar>31)
						{
							cout << "error in line no : " << codeinit[idx].second << endl;
							exit(0);
						}
						if(i==1)
						{
							temp.arg2 = ar;
						}
						else if(i==2)
						{
							temp.arg3 = ar;
						}
					}
					else if(i==3)
					{
						char minus = '+';
						if(word[0]=='-')
						{
							minus = word[0];
							word.erase(word.begin());
						}
						ll ar = 0;
        				for (int j = 0; j < word.size(); j++)
        				{
							if((word[j]-48)<0 || (word[j]-48)>9)
							{
								cout << "error in line no : " << codeinit[idx].second << endl;
								exit(0);
							}
        				    ar = 10 * ar + (word[j]-48) ;
        				}
						if(minus == '-')
						{
							ar = -1 * ar;
						}
						if(ar<-pow(2,11) || ar>=pow(2,11))
						{
							cout << "error in line no : " << codeinit[idx].second <<" Immidiate is out of Range"<< endl;
							exit(0);
						}
						temp.arg4 = ar;
					}
					i++;
				}
			}
		}
		else if(Format[index].format == "S")
		{
			int arg_flag = 0;
			int i = 1;
			while(ss >> word)
			{
				if(i==4)
				{
					cout << "error in line no : " << codeinit[idx].second << endl;
					exit(0);
				}
				else if(i==1)
				{
					word.erase(word.begin());
        			ll ar = 0;
        			for (int j = 0; j < word.size(); j++)
        			{
        			    ar = 10 * ar + (word[j]-48) ;
        			}
					temp.arg2 = ar;
				}
				else if(i==2)
				{
					regex exp1("[-]?[0-9]+");
					regex exp2("[-]?[0-9]+\\(x([0-9]|1[0-9]|2[0-9]|3[0-1])\\)");
					if(regex_match(word,exp1))
					{
						arg_flag = 1;
						char minus = '+';
						if(word[0]=='-')
						{
							minus = word[0];
							word.erase(word.begin());
						}
						ll ar = 0;
        				for (int j = 0; j < word.size(); j++)
        				{
							if((word[j]-48)<0 || (word[j]-48)>9)
							{
								cout << "error in line no : " << codeinit[idx].second << endl;
								exit(0);
							}
        				    ar = 10 * ar + (word[j]-48) ;
        				}
						if(minus == '-')
						{
							ar = -1 * ar;
						}
						if(ar<-pow(2,11) || ar>=pow(2,11))
						{
							cout << "error in line no : " << codeinit[idx].second <<" Immidiate is out of Range"<< endl;
							exit(0);
						}
						temp.arg3 = ar;
					}
					else if(regex_match(word,exp2))
					{
						char minus = '+';
						int s_index;
						if(word[0]=='-')
						{
							minus = word[0];
							word.erase(word.begin());
						}
						ll ar = 0;
        				for (int j = 0; j < word.size(); j++)
        				{
							if(word[j] == '(')
							{
								s_index = j;
								break;
							}
							if((word[j]-48)<0 || (word[j]-48)>9)
							{
								cout << "error in line no : " << codeinit[idx].second << endl;
								exit(0);
							}
        				    ar = 10 * ar + (word[j]-48) ;
        				}
						if(minus == '-')
						{
							ar = -1 * ar;
						}
						if(ar<-pow(2,11) || ar>=pow(2,11))
						{
							cout << "error in line no : " << codeinit[idx].second <<" Immidiate is out of Range"<< endl;
							exit(0);
						}
						temp.arg3 = ar;
						//.....
						word = word.substr(word.find("(")+1,word.size() - word.find("(") - 2);
						if(word[0] != 'x')
        				{
        				    cout << "error in line no : " << codeinit[idx].second << endl;
							exit(0);
        				}
						word.erase(word.begin());
        				ar = 0;
        				for (int j = 0; j < word.size(); j++)
        				{
							if((word[j]-48)<0 || (word[j]-48)>9)
							{
								cout << "error in line no : " << codeinit[idx].second << endl;
								exit(0);
							}
        				    ar = 10 * ar + (word[j]-48) ;
        				}
						if(ar<0 || ar>31)
						{
							cout << "error in line no : " << codeinit[idx].second << endl;
							exit(0);
						}
						temp.arg4 = ar;
					}
					else
					{
						cout << "error in line no : " << codeinit[idx].second << endl;
						exit(0);
					}
				}
				else if(i==3)
				{
					if(flag == 0)
					{
						cout << "error in line no : " << codeinit[idx].second << endl;
						exit(0);
					}
					if(word[0] == '(')
					{
						if(word[word.size()-1] != ')')
						{
							cout << "error in line no : " << codeinit[idx].second << endl;
							exit(0);
						}
						word.erase(word.begin());
						word.erase(word.end()-1);
					}
					if(word[0] != 'x')
        			{
        			    cout << "error in line no : " << codeinit[idx].second << endl;
						exit(0);
        			}
					word.erase(word.begin());
        			ll ar = 0;
        			for (int j = 0; j < word.size(); j++)
        			{
						if((word[j]-48)<0 || (word[j]-48)>9)
						{
							cout << "error in line no : " << codeinit[idx].second << endl;
							exit(0);
						}
        			    ar = 10 * ar + (word[j]-48) ;
        			}
					if(ar<0 || ar>31)
					{
						cout << "error in line no : " << codeinit[idx].second << endl;
						exit(0);
					}
					temp.arg4 = ar;
				}
				i++;
			}
		}
		else if(Format[index].format == "SB")
		{
			int i = 1;
			while(ss >> word)
			{
				if(i==4)
				{
					cout << "error in line no : " << codeinit[idx].second << endl;
					exit(0);
				}
				else if(i==1 || i==2)
				{
					if(word[0] != 'x')
        			{
        			    cout << "error in line no : " << codeinit[idx].second << endl;
						exit(0);
        			}
					word.erase(word.begin());
        			ll ar = 0;
        			for (int j = 0; j < word.size(); j++)
        			{
						if((word[j]-48)<0 || (word[j]-48)>9)
						{
							cout << "error in line no : " << codeinit[idx].second << endl;
							exit(0);
						}
        			    ar = 10 * ar + (word[j]-48) ;
        			}
					if(ar<0 || ar>31)
					{
						cout << "error in line no : " << codeinit[idx].second << endl;
						exit(0);
					}
					if(i==1)
					{
						temp.arg2 = ar;
					}
					else if(i==2)
					{
						temp.arg3 = ar;
					}	
				}
				else if(i==3)
				{
					int lab_flag = 0;
					int lab_index;
					for (int j = 0; j < Label.size(); j++)
					{
						if(word == Label[j].s)
						{
							lab_index = j;
							lab_flag = 1;
							temp.arg4 = (Label[lab_index].index - 1 - idx) * 4;
							break;
						}
					}
					if(lab_flag == 0)
					{
						int arg_flag = 1;
						char minus = '+';
						if(word[0]=='-')
						{
							minus = word[0];
							word.erase(word.begin());
						}
						ll ar = 0;
        				for (int j = 0; j < word.size(); j++)
        				{
							if((word[j]-48)<0 || (word[j]-48)>9)
							{
								cout << "error in line no : " << codeinit[idx].second << endl;
								exit(0);
							}
        				    ar = 10 * ar + (word[j]-48) ;
        				}
						if(minus == '-')
						{
							ar = -1 * ar;
						}
						if(ar<-pow(2,11) || ar>=pow(2,11))
						{
							cout << "error in line no : " << codeinit[idx].second <<" Immidiate is out of Range"<< endl;
							exit(0);
						}
						if(ar<(idx*-4) || ar%4 != 0)
						{
							cout << "error in line no : " << codeinit[idx].second << endl;
							exit(0);
						}
						temp.arg4 = ar;
						// cout << "error in line no : " << codeinit[idx].second << endl;
						// exit(0);
					}	
				}
				i++;
			}
		}
		else if(Format[index].format == "U")
		{
			int i = 1;
			while(ss >> word)
			{
				if(i==3)
				{
					cout << "error in line no : " << codeinit[idx].second << endl;
					exit(0);
				}
				else if(i==1)
				{
					word.erase(word.begin());
        			ll ar = 0;
        			for (int j = 0; j < word.size(); j++)
        			{
        			    ar = 10 * ar + (word[j]-48) ;
        			}
					temp.arg2 = ar;
				}
				else if(i==2)
				{
					regex expp1("[0-9]+");
					regex expp2("((0x[0]*[1-9A-Fa-f][0-9A-Fa-f]{0,4})|(0x[0]*))");
					if(regex_match(word,expp1))
					{
						ll ar =0;
						for (int j = 0; j<word.size() ; j++)
						{
							ar = ar * 10 + (word[j]-48);
						}
						if(ar<0 || ar>=pow(2,20))
						{
							cout << "error in line no : " << codeinit[idx].second <<" Immidiate is out of range"<< endl;
							exit(0);
						}
						temp.arg3 = ar;
					}
					else if(regex_match(word,expp2))
					{
						ll ar = hex_to_decimal_for_U_format(word);
						temp.arg3 = ar;
					}
					else
					{
						cout << "error in line no : " << codeinit[idx].second << endl;
						exit(0);
					}
				}
				i++;
			}
		}
		else if(Format[index].format == "UJ")
		{
			int i = 1;
			while(ss >> word)
			{
				if(i==3)
				{
					cout << "error in line no : " << codeinit[idx].second << endl;
					exit(0);
				}
				else if(i==1)
				{
					word.erase(word.begin());
        			ll ar = 0;
        			for (int j = 0; j < word.size(); j++)
        			{
        			    ar = 10 * ar + (word[j]-48) ;
        			}
					temp.arg2 = ar;
				}
				else if(i==2)
				{
					int lab_flag = 0;
					int lab_index;
					for (int j = 0; j < Label.size(); j++)
					{
						if(word == Label[j].s)
						{
							lab_index = j;
							lab_flag = 1;
							temp.arg3 = (Label[lab_index].index - 1 - idx) * 4;
							break;
						}
					}
					if(lab_flag == 0)
					{
						int arg_flag = 1;
						char minus = '+';
						if(word[0]=='-')
						{
							minus = word[0];
							word.erase(word.begin());
						}
						ll ar = 0;
        				for (int j = 0; j < word.size(); j++)
        				{
							if((word[j]-48)<0 || (word[j]-48)>9)
							{
								cout << "error in line no : " << codeinit[idx].second << endl;
								exit(0);
							}
        				    ar = 10 * ar + (word[j]-48) ;
        				}
						if(minus == '-')
						{
							ar = -1 * ar;
						}
						if(ar<-pow(2,19) || ar>=pow(2,19))
						{
							cout << "error in line no : " << codeinit[idx].second <<" Immidiate is out of Range"<< endl;
							exit(0);
						}
						if(ar<(idx*-4) || ar%4 != 0)
						{
							cout << "error in line no : " << codeinit[idx].second << endl;
							exit(0);
						}
						temp.arg3 = ar;
						// cout << "error in line no : " << codeinit[idx].second << endl;
						// exit(0);
					}
				}
				i++;
			}
		}
		code.pb(temp);
		idx++;
	}
}

string bin2hex(string binaryString) 
{
    // Ensure that the binary string is 32 bits long
    if (binaryString.length() != 32) 
	{
        return "Invalid binary string";
    }
    // Convert binary string to decimal
    std::bitset<32> binaryValue(binaryString);
    unsigned int intValue = binaryValue.to_ulong();
    // Convert integer to hexadecimal string
    std::stringstream hexStream;
    hexStream << hex << setw(8) << setfill('0') << intValue;
    string hexString = hexStream.str();
	transform(hexString.begin(), hexString.end(), hexString.begin(), ::toupper);
	hexString = "0x" + hexString;
	return hexString;
}

string dec2bin(int n) 
{
  	// Check for the special case of 0
  	if (n == 0) 
  	{
  	  return "00000";
  	}
  	string binaryResult;
  	// Loop to perform the conversion
  	while (n > 0) 
  	{
  	  // Append the least significant bit to the result
  	  binaryResult = (n % 2 == 0 ? "0" : "1") + binaryResult;   
  	  // Right shift the decimal number by 1 bit
  	  n /= 2;
  	}
  	while(binaryResult.size()<5)
  	{
		binaryResult = '0' + binaryResult;
  	}
  	return binaryResult;
}

string Dec2bin(int decimalNumber) 
{
  	bitset<12> binaryValue(decimalNumber);
    string binaryString = binaryValue.to_string();
	return binaryString;
}

string Dec2Bin(int decimalNumber) 
{
  	bitset<20> binaryValue(decimalNumber);
    string binaryString = binaryValue.to_string();
	return binaryString;
}

void R_machine_code(int i)
{
	//fucn7-rs2-rs1-func3-rd-opcode
  	string mcode,temp;
  	mcode = Format[code[i].indexs].opcode;
  	temp = dec2bin(code[i].arg2);
  	mcode  = temp + mcode;
  	mcode  = Format[code[i].indexs].fun3 + mcode;
  	temp = dec2bin(code[i].arg3);
  	mcode  = temp + mcode;
  	temp = dec2bin(code[i].arg4);
  	mcode  = temp + mcode; 
  	mcode  = Format[code[i].indexs].fun7 + mcode;
	string Mcode = bin2hex(mcode);
  	// now strore Mcode
	ofstream file;
	file.open("MCode.mc",std::ios_base::app);
	string myString = to_string(PC);
	PC = PC + 4;
	myString = convert(myString, 8);
	file << "0x" << myString << "  ";
	file << Mcode << endl;
	file.close();
}

void I_machine_code(int i)
{
	//immd-rs1-func3-rd-opcode
  	string mcode,temp;
  	mcode = Format[code[i].indexs].opcode;
  	temp = dec2bin(code[i].arg2);
  	mcode  = temp + mcode;
  	mcode  = Format[code[i].indexs].fun3 + mcode;
  	temp = dec2bin(code[i].arg3);
  	mcode  = temp + mcode;
  	temp   = Dec2bin(code[i].arg4);
  	mcode = temp + mcode;
  	string Mcode = bin2hex(mcode);
  	// now store Mcode
  	ofstream file;
	file.open("MCode.mc",std::ios_base::app);
	string myString = to_string(PC);
	PC = PC + 4;
	myString = convert(myString, 8);
	file << "0x" << myString << "  ";
	file<<Mcode<<endl;
	file.close();
}

void S_machine_code(int i)
{
  	//immd[11-5]-rs1-rs2-func3-immd[4-0]-opcode
  	// arg2 = rs1,arg3 = immd,arg4 = rs2
  	string mcode,temp;
  	mcode = Format[code[i].indexs].opcode;
  	temp = Dec2bin(code[i].arg3);
	reverse(temp.begin(), temp.end());
  	for(int j=0;j<5;j++)
  	{
  	  mcode  = temp[j] + mcode;
  	}
  	mcode  = Format[code[i].indexs].fun3 + mcode;
  	temp = dec2bin(code[i].arg4);
  	mcode  = temp + mcode;
  	temp = dec2bin(code[i].arg2);
  	mcode  = temp + mcode;
  	temp = Dec2bin(code[i].arg3);
	reverse(temp.begin(), temp.end());
  	for(int j=5;j<12;j++)
  	{
  	  mcode  = temp[j] + mcode;
  	}
  	string Mcode = bin2hex(mcode);
  	//now store Mcode
	ofstream file;
	file.open("MCode.mc",std::ios_base::app);
	string myString = to_string(PC);
	PC = PC + 4;
	myString = convert(myString, 8);
	file << "0x" << myString << "  ";
	file<<Mcode<<endl;
	file.close();
}

void SB_machine_code(int i)
{
  	//immd[12][10-5]-rs2-rs1-func3-immd[4-1][11]-opcode
  	// arg2 = rs1,arg4 = immd,arg3 = rs2
  	string mcode,temp;
  	mcode = Format[code[i].indexs].opcode;
  	temp = Dec2bin(code[i].arg4);
	if(temp[0] == '1')
	{
		temp = '1' + temp;
	}
	else
	{
		temp = '0' + temp;
	}
	temp.erase(temp.end() - 1);
	reverse(temp.begin(), temp.end());
  	mcode = temp[10] + mcode;
  	for(int j=0;j<4;j++)
  	{
  	  mcode  = temp[j] + mcode;
  	}
  	mcode  = Format[code[i].indexs].fun3 + mcode;
  	temp = dec2bin(code[i].arg2);
  	mcode  = temp + mcode;
  	temp = dec2bin(code[i].arg3);
  	mcode  = temp + mcode;
  	temp = Dec2bin(code[i].arg4);
	if(temp[0] == '1')
	{
		temp = '1' + temp;
	}
	else
	{
		temp = '0' + temp;
	}
	temp.erase(temp.end() - 1);
	reverse(temp.begin(), temp.end());
  	for(int j=4;j<10;j++)
  	{
  	  mcode  = temp[j] + mcode;
  	}
  	mcode = temp[11] + mcode;
  	string Mcode = bin2hex(mcode);
  	//now store mcode
	ofstream file;
	file.open("MCode.mc",std::ios_base::app);
	string myString = to_string(PC);
	PC = PC + 4;
	myString = convert(myString, 8);
	file << "0x" << myString << "  ";
	file<<Mcode<<endl;
	file.close();
}

void U_machine_code(int i)
{
  	//immd[20-0]-rd-opcode
  	// arg2 = rd,arg3 = immd,arg4 = blank
  	string mcode,temp;
  	mcode = Format[code[i].indexs].opcode;
  	temp = dec2bin(code[i].arg2);
  	mcode  = temp + mcode;
  	temp = Dec2Bin(code[i].arg3);
  	mcode = temp + mcode;
  	string Mcode = bin2hex(mcode);
  	//now store Mcode 
	ofstream file;
	file.open("MCode.mc",std::ios_base::app);
	string myString = to_string(PC);
	PC = PC + 4;
	myString = convert(myString, 8);
	file << "0x" << myString << "  ";
	file<<Mcode<<endl;
	file.close();
}

void UJ_machine_code(int i)
{
  	//immd[20][10-1][11][19-12]-rd-opcode
  	// arg2 = rd,arg3 = immd,arg4 = blank
  	string mcode,temp;
  	mcode = Format[code[i].indexs].opcode;
	temp = dec2bin(code[i].arg2);
  	mcode  = temp + mcode;
  	temp = Dec2Bin(code[i].arg3);
	if(temp[0] == '1')
	{
		temp = '1' + temp;
	}
	else
	{
		temp = '0' + temp;
	}
	temp.erase(temp.end() - 1);
	reverse(temp.begin(), temp.end());
  	for(int j=11;j<19;j++)
  	{
  	  mcode  = temp[j] + mcode;
  	}
  	mcode = temp[10] + mcode;
  	for(int j=0;j<10;j++)
  	{
  	  mcode  = temp[j]+mcode;
  	}
  	mcode = temp[19]+mcode;
  	string Mcode = bin2hex(mcode);
  	//now store Mcode
	ofstream file;
	file.open("MCode.mc",std::ios_base::app);
	string myString = to_string(PC);
	PC = PC + 4;
	myString = convert(myString, 8);
	file << "0x" << myString << "  ";
	file<<Mcode<<endl;
	file.close();
}	

void assembly_to_machine()
{
	for(int i=0;i<code.size();i++)
	{
		if(Format[code[i].indexs].format == "R")
	  	{
	  	  R_machine_code(i);
	  	}
	  	else if(Format[code[i].indexs].format == "UJ")
	  	{
	  	  UJ_machine_code(i);
	  	}
	  	else if(Format[code[i].indexs].format== "I")
	  	{
	  	  I_machine_code(i);
	  	}
	  	else if(Format[code[i].indexs].format== "S")
	  	{
	  	  S_machine_code(i);
	  	}
	  	else if(Format[code[i].indexs].format== "SB")
	  	{
	  	  SB_machine_code(i);
	  	}
	  	else if(Format[code[i].indexs].format== "U")
	  	{
	  	  U_machine_code(i);
	  	}
	}
}

int main(int argc, char const *argv[])
{
    storing_formates();
    for(int i=0;i<4000;i++)
	{
		datamemory[i] = "00";
	}

	ofstream filess;
	filess.open("MCode.mc");
	filess.close();

	read_data();
    read_inst();
	process();
	assembly_to_machine();

	ofstream file;
	file.open("MCode.mc",std::ios_base::app);
	string s = "-------------------------------------------------------";
	file<<s<<endl;

	//Print the Data Memory Part in Increasing Address Order
	ll counter = START;
	for(int i=3;i<400;i=i+4)
	{
		stringstream ss;
    	ss << hex << counter;
    	string hexValue = ss.str();
		counter = counter + 4;
		file << "0x" << hexValue << "  ";
		for (int j = i; j > i - 4; j--)
		{
			file << datamemory[j] << " ";
			if((j)%4==0)
			{
				file<<endl;
			}
		}		
	}
	file<<s<<endl;
	file.close();
    
    return 0;
}