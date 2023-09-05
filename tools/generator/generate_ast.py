import os
import sys

def generate_class(dir,name,fields):
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
#include "expression/expression.h"
#include "symbol/token.h"
#include <any>

namespace lox {{

class {name}:public AbstractExpression{{
    public:
    auto Accept(const ExpressionVisitor<std::any> &visitor)->std::any override{{
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
    with open(f"{dir}/{name}.h","w") as f:
        f.write(file_content)


def main():
    if len(sys.argv)!=2:
        print("Usage: generate_ast.py <output_directory>")
        return
    path=f"{os.path.dirname(os.path.realpath(__file__))}/grammer"
    header_files=["#pragma once"]
    with open(path,"r") as f:
        try:
            for line in f.readlines():
                line=line.strip()
                [symbol,attributes]=line.split(":")
                symbol=symbol.strip()
                header_files.append(f"#include \"{symbol}.h\"")
                attributes=attributes.split(",")
                generate_class(sys.argv[1],symbol,attributes)

        except:
            print("Invalid grammer file")
    header_files="\n".join(header_files)
    with open(f"{sys.argv[1]}/expression_headers.h","w") as f:
        f.write(header_files)

if __name__ == "__main__":
    main()