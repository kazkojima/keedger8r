SHELL = /bin/sh

PROGRAM = keedger8r
BUILD_DIR = .

LIB_CMXAS = str.cmxa unix.cmxa
INTEL_ML = Ast.ml Util.ml SimpleStack.ml Plugin.ml Preprocessor.ml CodeGen.ml
INTEL_MLL = Lexer.mll
INTEL_MLY = Parser.mly
GENERATED_ML = Lexer.ml Parser.mli Parser.ml

# Order matters.
CMXS = Ast.cmx Util.cmx SimpleStack.cmx Plugin.cmx Preprocessor.cmx \
	Parser.cmx Lexer.cmx \
	CodeGen.cmx \
	Emitter.cmx main.cmx

all: $(PROGRAM)

Lexer.ml: intel/Lexer.mll
	ocamllex -o $@ $<

Parser.mli Parser.ml: intel/Parser.mly
	ocamlyacc $<
	mv intel/Parser.mli intel/Parser.ml $(BUILD_DIR)

Parser.cmx: Parser.cmi

Parser.cmi: Parser.mli
	ocamlopt -c -keep-locs -bin-annot -I . -o $@ $< 

%.cmx: intel/%.ml
	ocamlopt -c -bin-annot -I . -o $@ $< 

%.cmx: %.ml
	ocamlopt -c -bin-annot -I . -o $@ $< 

$(PROGRAM): $(CMXS)
	ocamlopt $(LIB_CMXAS) $(CMXS) -o $(PROGRAM)

clean:
	rm -rf *.cmx *.cmi *.cmt *.cmti *.o $(GENERATED_ML) $(PROGRAM)
