
int balbrack(stack check) { 
  stack match = stack_create();

  while ((stack_empty(check) < 1) 
         && (stack_top(check) == '{'
         || stack_top(check) == '['
         || stack_top(check) == '(')) {
    stack_push(match, stack_pop(check));
  }

  while (stack_empty(check)) {
    if (stack_top(check) == stack_top(match)) {
      stack_pop(check);
      stack_pop(match);
    } else {
      return 0
    }
  }
  return 1;
  }
}
