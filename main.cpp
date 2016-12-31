#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<stdio.h>
#include<winsock2.h>
#include<string>
#include"Meta.h"
#include"parser.h"
#include"SQLResult.h"
#pragma comment(lib,"ws2_32.lib")
using namespace std;
vector<SQLResult> execute(string sql) {
	vector<SQLResult> result;
	AST* ast = parseSQLString(sql);
	if (!ast->isValid) {
		SQLResult result1;
		result1.valid = false;
		stringstream ss;
		ss << ast->errorMsg << "at (" << ast->errorLine << ", " << ast->errorColumn << ")";
		result1.info = ss.str();
		result.push_back(result1);
	}
	else {
		result = ast->execute();
	}
	delete ast;
	return result;
}
/*
int main(){
	system("del orderDB.lyx");
	ifstream in("initial.sql");
	string str((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
	vector<SQLResult> result = execute(str);
	ifstream in1("test.sql");
	string str1((istreambuf_iterator<char>(in1)), istreambuf_iterator<char>());
	vector<SQLResult> result1 = execute(str1);
	while (true) {
		char buffer[1024];
		cin.getline(buffer, 1024);
		if (std::string(buffer) == std::string("exit")) {
			break;
		}
		vector<SQLResult> result2 = execute(string(buffer));
	}
    return 0;
}*/
int main(int argc, char* argv[]) {
//	system("del orderDB.lyx");
//	ifstream in("initial.sql");
//	string str((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
//	vector<SQLResult> result = execute(str);
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0) {
		return 0;
	}
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET) {
		printf("socket error !");
		return 0;
	}
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR) {
		printf("bind error !");
	}
	if (listen(slisten, 5) == SOCKET_ERROR) {
		printf("listen error !");
		return 0;
	}
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[2048];
	while (true) {
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET) {
			printf("accept error !");
			continue;
		}
		int ret = recv(sClient, revData, 2048, 0);
		if (ret > 0) {
			revData[ret] = 0x00;
		}
		string input;
		string request = revData;
		if (request.substr(5, 7) == string("?fname=")) {
			int off = 12;
			while (revData[off] != ' ') {
				off++;
			}
			revData[off] = 0;
			input = string(revData + 12);
		}
		string query;
		for (int i = 0; i < input.size(); i++) {
			if (input[i] != '%'&&input[i] != '+') {
				stringstream stream;
				stream << input[i];
				query.append(stream.str());
			}
			if (input[i] == '+') {
				stringstream stream;
				stream << ' ';
				query.append(stream.str());
			}
			if (input[i] == '%') {
				int a = 0;
				int b = 0;
				if (input[i + 1] >= '0'&&input[i + 1] <= '9') {
					a = input[i + 1] - '0';
				}
				else {
					a = input[i + 1] - 'A' + 10;
				}
				if (input[i + 2] >= '0'&&input[i + 2] <= '9') {
					b = input[i + 2] - '0';
				}
				else {
					b = input[i + 2] - 'A' + 10;
				}
				i += 2;
				char c = (char)(a * 16 + b);
				stringstream stream;
				stream << c;
				query.append(stream.str());
			}
		}
		cout << "query:";
		cout << query << endl;
		if (query == std::string("exit")) {
			break;
		}
		vector<SQLResult> result2 = execute(query);
		string content;
		content += "<head><head><title>index.html</title></head><body>\n";
		content += query;
		if (query != "") {
			for (auto& item : result2) {
				if (item.valid) {
					content += "\nok\n";
				}
				else {
					content += "\nerror\n";
					content += item.info;
					content += "\n";
				}
				content += "<table border=\"1\">\n";
				content += "<tr>\n";
				for (auto& item1 : item.tableHeader) {
					content += "<td>\n";
					content += (item1.name + "(" + ((item1.isInt) ? "int" : "string") + ")");
					content += "</td>\n";
				}
				content += "</tr>\n";
				for (auto& item1 : item.table) {
					content += "<tr>\n";
					for (auto& item2 : item1) {
						content += "<td>\n";
						if (item2.nval) {
							content += "null";
						}
						else if (item2.isInt) {
							char bu[100];
							_itoa(item2.ival, bu, 10);
							content += bu;
						}
						else {
							content += item2.sval;
						}
						content += "</td>\n";
					}
					content += "</tr>\n";
				}
				content += "</table>\n";
			}
		}
		content += "<form method=\"get\">sql: <input type = \"text\" name = \"fname\"/><input type = \"submit\" value = \"Submit\" / ></ form>";
		content += "\n</body>";
		size_t length = content.size();
		char* response = new char[length + 256];
		sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n%s", (int)length, content.c_str());
		ret = send(sClient, response, (int)strlen(response), 0);
		closesocket(sClient);
	}
	closesocket(slisten);
	WSACleanup();
	return 0;
}
