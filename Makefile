BUILD_DIR := build
SYN := vivado
SYN_FLAGS := -mode batch -nojournal -notrace -source ../build_xsa.tcl

.PHONY: all
all: $(BUILD_DIR)/pmbus_zc702.xsa

$(BUILD_DIR)/pmbus_zc702.xsa: $(BUILD_DIR) *.sv *.tcl
	cd $(BUILD_DIR) && $(SYN) $(SYN_FLAGS)

$(BUILD_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)
