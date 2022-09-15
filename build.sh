clang++ main.cpp ir/lexer.cpp ir/token.cpp ir/expressions.cpp ir/statements.cpp parser/parser.cpp visitors/compiler.cpp \
 `llvm-config-10 --cxxflags --ldflags --system-libs --libs core` \
&& ./a.out