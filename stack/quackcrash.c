

void quacrash(stack crash) {

  stack keep, waste;
  keep = stack_create();
  waste = stack_create();

  while (stack_empty(crash) < 1) {
    int fresh = stack_pop(crash);
    if (stack_empty(keep) || (fresh != stack_top(keep))) {
      stack_push(keep, fresh);
    } else {
      int crashed = 1;
      stack_push(waste, fresh);
      while ((stack_empty(crash) < 1) && stack_top(crash) == fresh) {
        stack_push(waste, stack_pop(crash));
        crashed++;
      }
      while ((stack_empty(keep) < 1) && stack_top(keep) == fresh) {
          stack_push(waste, stack_pop(keep));
        crashed++;
      }
      int c = crashed % 4;
      if (c != 0) {
        for (int i = 0; i < c; i++) {
          stack_push(keep, stack_pop(waste));
        }
      }
    }
  }
  while (stack_empty(keep) < 1) {
    stack_push(crash, stack_pop(keep));
  }

  stack_destroy(keep);
  stack_destroy(waste);

  return;
}
