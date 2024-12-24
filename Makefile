install:
	clib install --dev

test:
	@$(CC) test.c -I src -I deps -o $@
	@./$@

.PHONY: test
