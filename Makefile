BUILD_DIR := build
CC := vitis
SYN := vivado
SYN_FLAGS := -mode batch -nojournal -notrace -source ../build_xsa.tcl

.PHONY: all
all: sw

.PHONY: sw
sw: $(BUILD_DIR)/pmbus_zc702.xsa *.py
	${CC} -s build_sw.py

$(BUILD_DIR)/pmbus_zc702.xsa: *.sv *.tcl
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && $(SYN) $(SYN_FLAGS)

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)
