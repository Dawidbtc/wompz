#include <iostream>
#include <string>
#include <queue>
#include <map>
using namespace std;
enum ValType {VINT,VREAL,VCHAR,VBOOL,VERR};

class Value{
    ValType type;
    bool Booltemp;
    int Inttemp;
    float Realtemp;
    string Stringtemp;

    public:
        Value():type(VERR),Inttemp(0),Realtemp(0.0){}
        Value(bool b):type(VBOOL),Booltemp(b),Inttemp(0),Realtemp(0.0) {}
        Value(int i):type(VINT),Inttemp(i){}
        Value(float f):type(VREAL),Inttemp(0),Realtemp(f){}
        Value(string s):type(VCHAR),Inttemp(0),Realtemp(0.0),Stringtemp(s){}

        ValType GetType() const{
            return type;
        }
        bool IsErr() const{ 
            return type==VERR;
        }
        bool IsInt() const{
            return type==VINT;
        }
        bool IsChar() const{
            return type==VCHAR;
        }
        bool IsReal() const{
            return type==VREAL;
        }
        bool IsBool() const{
            return type==VBOOL;
        }
        int GetInt() const{ 
            if(IsInt()){
                return Inttemp;
            }else{
                throw "Runtime Err: Val not a int";
            } 
        }
        string GetChar() const{
            if(IsChar()){
                return Stringtemp;
            }else{
                throw "Runtime Err: Val not a string";
            }
        }
        float GetReal() const{
            if(IsReal()){
                return Realtemp;
            }else{
                throw "Runtime Err: Val not a real";
            }
        }
        bool GetBool() const{
            if(IsBool()){
                return Booltemp;
            }else{
                throw "Runtime Err: Val not a bool";
            }
        }
        void SetType(ValType t){
            type = t;
        }
        void SetInt(int value){
            if(IsInt()){
                Inttemp=value;
            }else{
                throw "Runtime Err: Type not a int";
            }
        }
        void SetReal(float value){
            if(IsReal()){
                Realtemp=value;
            }else{
                throw "Runtime Err: Type not a real";
            }
        }
        void SetChar(string value){
            if(IsChar()){
                Stringtemp=value;
            }else{
                throw "Runtime Err: Type not a string";
            }
        }
        void SetBool(bool value){
            if(IsBool()){
                Booltemp=value;
            }else{
                throw "Runtime Err: Type not a bool";
            }
        }

        Value operator+(const Value& o) const;
        Value operator-(const Value& o) const;
        Value operator*(const Value& o) const;
        Value operator/(const Value& o) const;
        Value operator==(const Value& o) const;
        Value operator<(const Value& o) const;

        friend ostream& operator<<(ostream& out, const Value& o){
            if(o.IsInt()){
                out<<o.Inttemp;
            }
            else if(o.IsChar()){
                out<<o.Stringtemp;
            }
            else if(o.IsReal()){
                out<<o.Realtemp;
            }
            else{
                out<<"ERROR";
            }
            return out;
        }
};