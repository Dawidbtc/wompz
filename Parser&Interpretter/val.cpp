#include "val.h"
//value obj operator overloading
Value Value::operator+(const Value& o) const{
    if(GetType()==o.GetType()){
        if(IsInt()){
            return Value(Inttemp+o.GetInt());
        }
        if(IsReal()){
            return Value(Realtemp+o.GetReal());
        }
    }
    else if(IsInt()&&o.IsReal()){
        return Value((float)GetInt()+o.GetReal());
    }
    else if(IsReal()&&o.IsInt()){
        return Value(GetReal()+(float) o.GetInt());
    }
    return Value();
}

Value Value::operator-(const Value& o) const{
    if(GetType()==o.GetType()){
        if(IsInt()){
            return Value(Inttemp-o.GetInt());
        }
        if(IsReal()){
            return Value(Realtemp-o.GetReal());
        }
    }
    else if(IsInt() && o.IsReal()){
        return Value((float)GetInt()-o.GetReal());
    }
    else if(IsReal()&&o.IsInt()){
        return Value(GetReal()-(float)o.GetInt());
    }
    return Value();
}

Value Value::operator*(const Value& o) const{
    if(GetType()==o.GetType()){
        if(IsInt()){
            return Value(Inttemp*o.GetInt());
        }
        if(IsReal()){
            return Value(Realtemp*o.GetReal());
        }
    }
    else if(IsInt()&&o.IsReal()){
        return Value((float)GetInt()*o.GetReal());
    }
    else if(IsReal()&&o.IsInt()){
        return Value(GetReal()*(float)o.GetInt());
    }
    return Value();
}

Value Value::operator/(const Value& o) const{
    if(GetType()==o.GetType()){
        if(IsInt()){
            return Value(Inttemp/o.GetInt());
        }
        if(IsReal()){
            return Value(Realtemp/o.GetReal());
        }
    }
    else if(IsInt()&&o.IsReal()){
        return Value((float)GetInt()/o.GetReal());
    }
    else if(IsReal()&&o.IsInt()){
        return Value(GetReal()/((float)o.GetInt()));
    }
    return Value();
}

Value Value::operator==(const Value& o) const{
    if(GetType()==o.GetType()){
        if(IsInt()){
            return Value((bool)(Inttemp==o.GetInt()));
        }
        if(IsReal()){
            return Value((bool)(Realtemp==o.GetReal()));
        }
        if(IsChar()){
            return Value((bool)(Stringtemp<o.GetChar()));
        }
    }
    else if(IsInt()&&o.IsReal()){
        return Value((bool)(((float)GetInt())==o.GetReal()));
    }
    else if(IsReal()&&o.IsInt()){
        return Value((bool)(GetReal()==((float)o.GetInt())));
    }
    return Value();
}

Value Value::operator<(const Value &o) const{
    if(GetType()==o.GetType()){
        if(IsInt()){
            return Value(Inttemp<o.GetInt());
        }
        if(IsReal()){
            return Value(Realtemp<o.GetReal());
        }
        if(IsChar()){
            return Value(Stringtemp<o.GetChar());
        }
    }
    else if(IsInt()&&o.IsReal()){
        return Value(((float)GetInt())<o.GetReal());
    }
    else if(IsReal()&&o.IsInt()){
        return Value(GetReal()<((float)o.GetInt()));
    }
    return Value();
}