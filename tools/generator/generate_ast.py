import os
import sys

def generate_class(dir,name,fields,type):
    field_content=[]
    constructor_args=[]
    constructor_intialize=[]
    for field in fields:
        [field_type,field_name]=field.strip().split(" ")
        field_type=field_type.strip()
        field_name=field_name.strip()
        field_content.append(f"std::unique_ptr<{field_type}> {field_name}_;")
        constructor_args.append(f"std::unique_ptr<{field_type}> {field_name}_")
        constructor_intialize.append(f"{field_name}_(std::move({field_name}_))")
        
    field_content="\n".join(field_content)
    constructor_args=",\n".join(constructor_args)
    constructor_intialize=",".join(constructor_intialize)

    file_content=f"""
#pragma once

#include <memory>
#include "statement/statement.h"
#include "expression/expression.h"
#include "symbol/types.h"
#include <any>
#include <vector>
#include "parser/parser.h"

namespace lox {{

class {name}:public Abstract{type}{{
    public:
    auto Accept(const {type}Visitor<LoxTypes> &visitor)->LoxTypes override{{
        return visitor.Visit(*this);
    }}
    explicit {name}(
        {constructor_args}
    ):{constructor_intialize}{{}}
    ~{name}()override=default;
    
    {field_content}
}};

}}  // namespace lox
    """
    
    with open(f"{dir}/{type.lower()}/{name}.h","w") as f:
        f.write(file_content)



def main():
    
    if len(sys.argv)!=3:
        print("Usage: generate_ast.py <output_directory> <type>")
        return
    type=sys.argv[2]
    path=f"{os.path.dirname(os.path.realpath(__file__))}/{type.lower()}"
    header_files=["#pragma once"]
    with open(path,"r") as f:
        try:
            for line in f.readlines():
                line=line.strip()
                [symbol,attributes]=line.split("#")
                symbol=symbol.strip()
                header_files.append(f"#include \"{type.lower()}/{symbol}.h\"")
                attributes=attributes.split(",")
                generate_class(sys.argv[1],symbol,attributes,type.capitalize())

        except:
            print("Invalid grammer file")
    header_files="\n".join(header_files)
    with open(f"{sys.argv[1]}/{type.lower()}/{type.lower()}_headers.h","w") as f:
        f.write(header_files)

if __name__ == "__main__":
    main()