#include <iostream>
#include <fstream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include <node/node.h>
#include <node/v8.h>

using namespace v8;
using namespace std;

std::string argToString(const Arguments & args, int n)
{
  char * buff;
  std::string str;

  buff = new char[args[n]->ToString()->Length() + 1];
  memset(buff, 0, args[n]->ToString()->Length() + 1);
  args[n]->ToString()->WriteAscii(buff);
  str.assign(buff);
  delete buff;
  return (str);
}

Handle<Value> Move(const Arguments& args) {
  HandleScope   scope;
  std::string from;
  std::string to;
  char buffer[1024];
  int fromFile = -1;
  int toFile = -1;
  int rd = -1;

  if (args.Length() != 2)
  {
    ThrowException(Exception::TypeError(
          String::New("Wrong number of arguments")));
    return scope.Close(Undefined());
  }
  if (!args[0]->IsString() || !args[1]->IsString())
  {
    ThrowException(Exception::TypeError(
          String::New("Wrong arguments")));
    return scope.Close(Undefined());
  }

  from = argToString(args, 0);
  to = argToString(args, 1);

  if (rename(from.c_str(), to.c_str()) == 0)
  {
    return scope.Close(Undefined());
  }

  fromFile = open(from.c_str(), O_RDONLY);
  toFile = open(to.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0640);
  if (fromFile == -1)
  {
    ThrowException(Exception::TypeError(
          String::New("Can't open 'from' file")));
    return scope.Close(Undefined());
  }
  if (toFile == -1)
  {
    ThrowException(Exception::TypeError(
          String::New("Can't open 'to' file")));
    return scope.Close(Undefined());
  }
  memset(buffer, 0, 1024);
  do
  {
    rd = read(fromFile, buffer, 1023);
    buffer[rd] = '\0';
    write(toFile, buffer, rd);
  } while (rd > 0);
  close(fromFile);
  close(toFile);
  if (unlink(from.c_str()) != 0)
  {
    ThrowException(Exception::TypeError(
          String::New("Can't unlink 'from' file")));
    return scope.Close(Undefined());
  }
  return scope.Close(Undefined());
}

void init(Handle<Object> target) {
  target->Set(String::NewSymbol("move"),
      FunctionTemplate::New(Move)->GetFunction());
}
NODE_MODULE(movefile, init)

