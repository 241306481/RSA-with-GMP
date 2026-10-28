//-std=c++17 -Wl,--stack=67108864 -pthread -finput-charset=GBK -fexec-charset=GBK -Ofast -s -fstack-protector-all
//-static -lgmpxx -lgmp -lbcrypt
#include<bits/stdc++.h>
#include<io.h>
#include<gmpxx.h>
#include<windows.h>
using namespace std;using ll=long long;
const int M=200;int cnt=max(4u,min(thread::hardware_concurrency(),16u)),cntt;ll totol,bytes,tmp;atomic<ll>now,tot;
mt19937 rnd;mpz_class n,d,p,q,dp,dq,qinv;queue<string>input[256];string a,in="密文.txt",out="明文.txt",output2[256];double sstim,stim,tim;
queue<pair<short,string>>output[256];ifstream fin;ofstream fout;atomic<bool>running(true);bool silent,in_binary,out_binary,crt;
inline mpz_class fast_pow(const mpz_class &x,const mpz_class &y,const mpz_class &m=n){mpz_class ans;
	mpz_powm(ans.get_mpz_t(),x.get_mpz_t(),y.get_mpz_t(),m.get_mpz_t());return ans;}
const char c[95]="w7)OE$_AT%l;\'smgLu,2krNFieWVJ`p-#{ZQ.t0q}j5K8!\\G=XI*M?S4hbc9nf/]|vz@:6~B(<U^Co\"3H[+x>RdPaYyD1&";
const char c2[65]="3dIgAVkjcrH+8wxPSU5vTJBzNReYMtO6mlqQh92uKpW7FGoZnb1yEfL=sD4CX0ia";
inline mpz_class decompress(const string &s){mpz_class num=0;for(char a:s)num=num*94+mpz_class(int(find(c,c+95,a)-c));return num;}
inline mpz_class decompress2(const string &s){mpz_class num=0;for(char a:s)num=(num<<6)+mpz_class(int(find(c2,c2+64,a)-c2));return num;}
inline bool isconsole(){
	static char b=-1;if(~b)return b;
	HANDLE h=GetStdHandle(STD_INPUT_HANDLE);DWORD mode;
	return b=h!=INVALID_HANDLE_VALUE&&GetConsoleMode(h,&mode)!=0;
}
inline void Output(const wstring s){
	DWORD bytesWritten;HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsoleW(hConsole,s.c_str(),s.size(),&bytesWritten,NULL);
}
inline wstring to_wstring(const string &s){
	int len=MultiByteToWideChar(CP_ACP,0,s.c_str(),s.size(),nullptr,0);if(len<=0)return L"";
	wstring ws(len,L'\0');MultiByteToWideChar(CP_ACP,0,s.c_str(),s.size(),&ws[0],len);return ws;
}
struct Print{
	Print(int x=0):err(x){}
	~Print(){flush();}
	bool err;wostringstream wsout;
	template<class T>Print& operator<<(const T &x){wsout<<x;return *this;}
	Print& operator<<(const string &s){wsout<<to_wstring(s);return *this;}
	Print& operator<<(const char *s){wsout<<to_wstring(s);return *this;}
	Print& operator<<(Print& (*manip)(Print&)){return manip(*this);}
	Print& flush(){if(err||!silent)Output(wsout.str());wsout.clear();wsout.str(L"");return *this;}
}print(1);
Print& flush(Print &a){return a.flush();}
inline void sleep(const int &x){this_thread::sleep_for(chrono::milliseconds(x));}
inline ll get_time(){using namespace chrono;return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();}
inline wstring to_process(const ll &a,const ll &b){
	wostringstream wsout;wsout<<fixed<<setprecision(3)<<L"进度: "<<(b==0?100:a*100.0/b)<<"% ( "<<a<<" / "<<b<<" )";return wsout.str();
}
inline wstring to_time(){wostringstream wsout;
	wsout<<fixed<<setprecision(3)<<L"耗时: "<<tim-stim<<"s ( "<<int((tim-stim)/3600)<<"h "<<int((tim-stim)/60)%60<<"m "<<fmod((tim-stim),60)<<L"s )";
	if(sstim)wsout<<L" | 总耗时："<<tim-sstim<<"s ( "<<int((tim-sstim)/3600)<<"h "<<int((tim-sstim)/60)%60<<"m "<<fmod(tim-sstim,60)<<"s )";
	return wsout.str();
}
[[noreturn]] inline void my_pause(){
	if(isconsole()&&!silent){
		print<<L"请按回车键继续..."<<flush;ll a,b;
		do a=get_time(),getline(cin,::a),b=get_time();while(b-a<1&&cin.good());
	}exit(0);
}
template<typename T>inline void open(T &f,const string &s,const ios_base::openmode &mode,const bool &bin){
	f.open(s,bin?mode|ios::binary:mode);if(!f.is_open())print<<L"无法打开 \""<<s<<L"\" ，请检查访问权限，确保文件存在\n"<<flush,my_pause();
}
template<class T>inline void create(T &&f){
	for(int i=0;i<cnt;i++){try{thread(f,i).detach();}catch(const exception &e){running=false,
		print<<L"创建第 "<<i+1<<L" 个线程失败\n请尝试重启此程序\n详细信息："<<e.what()<<"\n"<<flush,my_pause();}}
}
inline mpz_class zh(const string &s){
	mpz_class y;if(in_binary)mpz_import(y.get_mpz_t(),s.size(),-1,1,0,0,s.data());else y=decompress(s);
	if(crt){const mpz_class z=fast_pow(y,dq,q);y=(fast_pow(y,dp,p)-z)*qinv;
		mpz_mod(y.get_mpz_t(),y.get_mpz_t(),p.get_mpz_t());return z+y*q;}else return fast_pow(y,d);
}
inline mpz_class read(const string &s){mpz_class x;istringstream sin(s);sin>>x;return x;}
inline int read2(const string &s){int x;istringstream sin(s);sin>>x;return x;}
inline void read_key(const string&s,mpz_class f(const string&)){
	n=f(s.substr(1,s.find(',')-1));d=f(s.substr(s.find(',')+1,s.size()-s.find(',')-2));
}
inline void read_crt(const string &s,mpz_class f(const string&)){
	string pp,qq,dpp,dqq;istringstream sin(s.substr(1,s.size()-2));
	getline(sin,pp,',');getline(sin,qq,',');getline(sin,dpp,',');getline(sin,dqq,',');
	p=f(pp),q=f(qq),dp=f(dpp),dq=f(dqq);
}
inline bool check(const string &s){return !s.empty()&&s.find_first_not_of("0123456789")==string::npos;}
inline bool check_key(const string &s,const bool &comp){
	if(s.empty()||s.front()!='('||s.back()!=')'||s.find(',')==string::npos||s.find_first_of(',')!=s.find_last_of(','))return 0;
	const string nn=s.substr(1,s.find(',')-1),ee=s.substr(s.find(',')+1,s.size()-s.find(',')-2);mpz_class n,e;
	auto chk=[&comp](const string &s)->bool{return s.find_first_not_of(comp?c2:"0123456789")==string::npos;};
	auto dec=[&comp](const string &s)->mpz_class{return comp?decompress2(s):read(s);};
	return chk(nn)&&chk(ee)&&(n=dec(nn))>>2000!=0&&(e=dec(ee),1<e&&e<n);
}
inline bool check_crt(const string &s,const bool &comp){
	if(s.empty()||s.front()!='('||s.back()!=')'||count(s.begin(),s.end(),',')!=3)return 0;
	auto chk=[&comp](const string &s)->bool{return s.find_first_not_of(comp?c2:"0123456789")==string::npos;};
	auto dec=[&comp](const string &s)->mpz_class{return comp?decompress2(s):read(s);};
	string pp,qq,dpp,dqq;mpz_class p,q,dp,dq;istringstream sin(s.substr(1,s.size()-2));
	getline(sin,pp,',');getline(sin,qq,',');getline(sin,dpp,',');getline(sin,dqq,',');
	return chk(pp)&&chk(qq)&&chk(dpp)&&chk(dqq)&&(p=dec(pp))!=(q=dec(qq))&&p>>1000!=0&&mpz_probab_prime_p(p.get_mpz_t(),25)
		&&q>>1000!=0&&mpz_probab_prime_p(q.get_mpz_t(),25)&&(dp=dec(dpp),0<dp&&dp<p-1)&&(dq=dec(dqq),0<dq&&dq<q-1);
}
inline void init_mt19937(mt19937 &rnd){
	unsigned int data[624];
	if(!BCRYPT_SUCCESS(BCryptGenRandom(NULL,(PUCHAR)data,sizeof(data),BCRYPT_USE_SYSTEM_PREFERRED_RNG))){
		random_device rd;for(unsigned int &x:data)x=rd()^get_time();
	}seed_seq seed(data,data+624);rnd.seed(seed);
}
void work(int id){
	while(!input[id].empty()){
		mpz_class x=zh(input[id].front());const int size=mpz_class(x>>8*M&255).get_si();
		input[id].pop();output[id].push(make_pair(mpz_class(x>>8*M+8&255).get_si(),string()));
		for(int i=M*8-8;i>=max(0,M*8-size*8);i-=8)output[id].back().second.push_back(mpz_class(x>>i&255).get_si());
		tot.fetch_add(size,memory_order_relaxed);now.fetch_add(1,memory_order_relaxed);Sleep(0);if(!running)break;
	}
}
inline void clear(mpz_class &x){mpz_ptr z=x.get_mpz_t();if(z->_mp_d)SecureZeroMemory(z->_mp_d,z->_mp_alloc*sizeof(mp_limb_t));x=0;}
inline string tolower(string s){for(char &c:s)c=tolower(c);return s;}
inline void read_arg(const int argc,const char *const argv[]){
	string s;mpz_class x;
	for(int i=1,j=1;i<argc;j=++i){
		auto need=[&](const string &s){print<<L"警告：位于第 "<<j<<" 个参数："<<a<<" 后期望 "<<s<<" ，但没有找到\n"<<flush;};
		auto fail=[&](){print<<L"警告：位于第 "<<j<<" 个参数："<<a<<" 指定的值无效\n"<<flush;};
		if(a=argv[i],a=="-h"||a=="-help"){
			print<<LR"helpinfo(
    -h, -help              显示帮助
    -silent                不输出信息
    -threads <N>           指定线程数（ 1-256 ），若不指定或无效则使用默认值
    -in   <filepath>       指定密文路径
    -in-binary             以二进制读取密文
    -out  <filepath>       指定明文路径
    -out-binary            以二进制写入明文
    -key                   指定 (n,d) 的十进制
    -KEY                   指定 (n,d) 的压缩串
    -crt                   指定 (p, q, dp, dq) 的十进制
    -CRT                   指定 (p, q, dp, dq) 的压缩串
    -n                     指定 n 的十进制
    -N                     指定 n 的压缩串
    -d                     指定 d 的十进制
    -D                     指定 d 的压缩串
        (-s) <string>      指定 key, crt, n, d 的读取字符串，必须紧跟于相应参数后
        -f <filepath>      指定 key, crt, n, d 的读取文件，必须紧跟于相应参数后
    若有 -h, -help ，则忽略其它参数
    若没有指定 -in, -out ，则使用默认值
    若出现重复项，以最后生效的为准
    若有指定 crt ，则忽略 n, d
    有效的 n 需满足：大于 2^2000
    有效的 p, q 需满足：大于 2^1000 且为质数
    有效的 dp, dq 需满足：分别位于 [1,p-1) 与 [1,q-1)
    本程序不会检查 n, d 的有效性
)helpinfo"<<flush,exit(0);
		}else if(a=="-silent")silent=1;
		else if(a=="-threads"){
			if(++i==argc){need("<N>");continue;}if(!check(a=argv[i])||a.size()>3
				||(j=read2(a),j==0||j>256)){j=i-1;fail();continue;}cnt=j;
		}else if(a=="-in"||a=="-out"){
			if(++i==argc){need("<filepath>");continue;}if(a=="-in")in=argv[i];else out=argv[i];
		}else if(a=="-in-binary")in_binary=1;else if(a=="-out-binary")out_binary=1;
		else if(s=tolower(a),a=="-key"||a=="-KEY"||a=="-crt"||a=="-CRT"||s=="-n"||s=="-d"){
			if(++i==argc){need("(-s) <string>, -f <filepath>");continue;}
			if(string(argv[i])=="-f"){
				if(++i==argc){j=i-1;need("<filepath>");continue;}ifstream fin(argv[i]);
				if(!fin.is_open()){print<<L"警告：位于第 "<<i<<" 个参数：打开指定的文件 "<<argv[i]<<" 失败\n"<<flush;continue;}
				if(!(fin>>s)){print<<L"警告：位于第 "<<i<<" 个参数：读取文件 "<<argv[i]<<" 失败\n"<<flush;continue;}fin.close();
			}else{if(string(argv[i])=="-s"&&++i==argc){j=i-1;need("<string>");continue;}s=argv[i];}
			if(a=="-key"&&!check_key(s,0)||a=="-KEY"&&!check_key(s,1)||a=="-crt"&&!check_crt(s,0)||a=="-CRT"&&!check_crt(s,1)||
				a.size()<3&&(islower(a[1])&&!check(s)||s.find_first_not_of(c2)!=string::npos)){fail();continue;}
			if(a.size()>2)
				if(a=="-key")read_key(s,read);else if(a=="-KEY")read_key(s,decompress2);
				else{if(a=="-crt")read_crt(s,read);else read_crt(s,decompress2);crt=1;}
			else{
				if(islower(a[1]))x=read(s);else x=decompress2(s);s=tolower(a[1]);
				if(s=="n"&&x>>2000==0){fail();continue;}if(s=="n")n=x;else d=x;
			}
		}else print<<L"警告：位于第 "<<i<<" 个参数：未知的 "<<a<<"\n"<<flush;
	}
	if(crt)d=0;else{
		if(n&&!d)n=0,print<<L"警告：指定了 n ，但没有指定 d\n"<<flush;
		if(!n&&d)d=0,print<<L"警告：指定了 d ，但没有指定 n\n"<<flush;
	}if(in==out)print<<L"警告：读取文件与写入文件一致，将会覆盖写入\n"<<flush;
}
int main(int argc,char *argv[]){
	read_arg(argc,argv);print.err=0;sstim=get_time()/1000.0;init_mt19937(rnd);
	if(!crt&&!n){
		if(!isconsole())print.err=1,print<<L"错误：检测到当前非交互式终端，\n请使用命令行参数，或使用交互式终端\n"<<flush,exit(1);
		print<<L"请选择输入的私钥类别：\n1. NORMAL (n, d)\n2. CRT (p, q, dp, dq)\n"<<flush;getline(cin,a),cin.clear();
		while(a!="1"&&a!="2")print<<L"输入无效，请重新输入："<<flush,getline(cin,a),cin.clear();if(a=="2")crt=1;
		print<<L"请选择输入的私钥格式：\n1. 十进制\n2. 压缩串\n"<<flush;getline(cin,a),cin.clear();bool comp=0;
		while(a!="1"&&a!="2")print<<L"输入无效，请重新输入："<<flush,getline(cin,a),cin.clear();if(a=="2")comp=1;
		print<<L"请输入私钥："<<flush,getline(cin,a),cin.clear();
		while(!(crt?check_crt:check_key)(a,comp))print<<L"输入无效，请重新输入："<<flush,getline(cin,a),cin.clear();
		(crt?read_crt:read_key)(a,comp?decompress2:(mpz_class(*)(const string&))read);
	}
	if(crt)n=p*q,mpz_invert(qinv.get_mpz_t(),q.get_mpz_t(),p.get_mpz_t());
	print<<L"统计中...\n";open(fin,in,ios::in,in_binary);if(in!=out)open(fout,out,ios::out,out_binary);
	if(in_binary)bytes=(mpz_sizeinbase(n.get_mpz_t(),2)+7)/8;stim=get_time()/1000.0;
	{
		ll lst=0,t;
		if(in_binary){
			vector<char>buf(bytes*4096);ll got,full,rest,idx=0;
			while(fin.read(buf.data(),buf.size()),(got=fin.gcount())>0){
				full=got/bytes,rest=got-full*bytes;
				for(ll k=0;k<full;k++)input[idx].emplace(buf.data()+k*bytes,bytes),idx=(idx+1)%cnt;
				totol+=full+(rest>0);
				if(rest){
					string last(buf.data()+full*bytes,rest);last.resize(bytes);
					for(ll i=rest;i<bytes;i++)last[i]=rnd();
					input[idx].push(move(last)),idx=(idx+1)%cnt;break;
				}
				if((t=get_time())-lst>=300)lst=t,tim=t/1000.0,
					print<<L"进度: 已找到 "<<totol<<L" 块 | "<<to_time()<<"\r"<<flush;
			}
		}else while(fin>>a){
			input[totol%cnt].push(a);++totol;
			if((t=get_time())-lst>=300)lst=t,tim=t/1000.0,
				print<<L"进度: 已找到 "<<totol<<L" 块 | "<<to_time()<<"\r"<<flush;
		}
	}
	tim=get_time()/1000.0;fin.close();print<<L"进度: 已找到 "<<totol<<L" 块 | "<<to_time()<<L"\n";if(in==out)open(fout,out,ios::out,out_binary);
	print<<L"统计完毕，解密中（线程数："<<cnt<<L"）...\n"<<flush;stim=get_time()/1000.0;create(work);
	while((tmp=now)<totol)tim=get_time()/1000.0,print<<to_process(tmp,totol)<<" | "<<to_time()<<"\r"<<flush,sleep(300);
	tim=get_time()/1000.0;print<<to_process(totol,totol)<<" | "<<to_time()<<L"\n解密完毕，写入文件中...\n"<<flush;
	clear(n);clear(d);clear(p);clear(q);clear(dp);clear(dq);clear(qinv);tmp=0;stim=tim;
	for(ll i=0;i<totol;i++){const auto p=output[i%cnt].front();output[i%cnt].pop();output2[p.first]+=p.second,cntt=max(cntt,p.first+1);}
	{
		string buf;buf.reserve(1<<20);ll lst=0,t;
		for(ll i=0;i<tot;i++){
			if(i/cntt>=output2[i%cntt].size())buf.push_back(char(rnd()&255));else buf.push_back(output2[i%cntt][i/cntt]);
			if(buf.size()>=(1<<20)){
				fout.write(buf.data(),buf.size()),buf.clear();
				if((t=get_time())-lst>=300)lst=t,tim=t/1000.0,
					print<<to_process(i,tot)<<" | "<<to_time()<<"\r"<<flush;
			}
		}
		fout.write(buf.data(),buf.size());
	}
	for(int i=0;i<cntt;i++)string().swap(output2[i]);fout.close();tim=get_time()/1000.0;
	print<<to_process(tot,tot)<<" | "<<to_time()<<L"\n写入完毕！可以关闭此程序\n",my_pause();
	return 0;
}
