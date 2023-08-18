#include "http_server.hh"

#include <vector>

#include <sys/stat.h>
#include<iostream>
#include <fstream>
#include <sstream>

using namespace std;
vector<string> split(const string &s, char delim) {
  vector<string> elems;

  stringstream ss(s);
  string item;

  while (getline(ss, item, delim)) {
    if (!item.empty())
      elems.push_back(item);
  }

  return elems;
}

HTTP_Request::HTTP_Request(string request) {
  vector<string> lines = split(request, '\n');
  vector<string> first_line = split(lines[0], ' ');

  this->HTTP_version = "1.0"; // We'll be using 1.0 irrespective of the request

  /*
   TODO : extract the request method and URL from first_line here
  */
  this->method=first_line[0];
  this->url=first_line[1];

  if (this->method != "GET") {
    cerr << "Method '" << this->method << "' not supported" << endl;
    exit(1);
  }
}

HTTP_Response *handle_request(string req) {
  HTTP_Request *request = new HTTP_Request(req);

  HTTP_Response *response = new HTTP_Response();

  string url = string("html_files") + request->url;
   
  response->HTTP_version = "HTTP/1.1";
   //cout<<"we are in handle function\n"<<endl;
  struct stat sb;
  if (stat(url.c_str(), &sb) == 0) // requested path exists
  {
    cout<<"path exist\n"<<endl;
    response->status_code = "200";
    response->status_text = "OK";
    response->content_type = "text/html";

    string body;

    if (S_ISDIR(sb.st_mode)) {
      /*
      In this case, requested path is a directory.
      TODO : find the index.html file in that directory (modify the url
      accordingly)
      */
     if(url[url.length()-1]!='/')
      url.append("/");
     cout<<"path exist\n"<<endl;
     url=url+string("index.html");
    }

    /*
    TODO : open the file and read its contents
    */
    ifstream in(url);
    string line;

    while(in)
    {
      getline(in,line);
      body=body+line;
      body=body+"\n";

      
    }
    //cout<<body<<endl;
    in.close();
    //in>>body;
    response->body=body;
    //cout<<"path exist\n"<<endl;
    /*
    TODO : set the remaining fields of response appropriately
    */
  }

  else {
    response->status_code = "404";
    response->status_text= "Not Found";
    response->content_type = "text/html";

    /*
    TODO : set the remaining fields of response appropriately
    */
  }

  delete request;

  return response;
}

string HTTP_Response::get_string() {
  /*
  TODO : implement this function
  */
 string str;
if(this->status_code=="200"){
  str=this->HTTP_version+" "+this->status_code+" "+this->status_text+"\n"
 +"Content-Length: "+to_string(this->body.length())+"\n"
 +"Content-Type: text/html"+"\n\n"
 +this->body;
}
else{
  this->body=R"(<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Server Not Found</title>
</head>

<body>
    <h1>404 page not found:(</h1>

</body>

</html>)";
this->body=body+"\n";
  str=this->HTTP_version+" "+this->status_code+" "+this->status_text+"\n"
 +"Content-Length: "+to_string(this->body.length())+"\n"
 +"Content-Type: text/html"+"\n\n"
 +this->body;
}
 return str;

}
