SRC_DIR = ./src
OUT_DIR = ./build
DOC_DIR = $(OUT_DIR)/doc


.DEFAULT_GOAL := all

$(OUT_DIR):
	mkdir "$(OUT_DIR)"	

$(DOC_DIR) : $(OUT_DIR)
	mkdir "$(DOC_DIR)"	

clean: $(OUT_DIR)
	rm -r $(OUT_DIR)

doc: $(DOC_DIR)
	doxygen doxyfile