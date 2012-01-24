dynamic global_event_func;
dynamic[] global_args;

class EventState {
	dynamic func;
	dynamic[] args;
	String[] types;
	String funcType;

	EventState(dynamic f, dynamic[] args) {
		func = f;
		_args = args;
	}
}

class EventExecutor {
	int state_count;
	int cur_state_num;
	EventState[] states;
	int state_num;

	EventExecutor() {
		state_count = 0;
		states = [];
	}
	
	void add(EventState state) {
		states.add(state);
		state_count++;
		String funcType = "Func<";
		foreach (dynamic arg in state.args) {
			String type = "%s"(arg).split(":")[0];
			state.types << type;
			funcType += type + ", ";
		}
		funcType = funcType[0 to funcType.size - 3] + ">";
		state.funcType = funcType;
		OutputStream ous = new OutputStream("state_log.log", "a+");
		ous.println("state:" + state_count);
		ous.flush();
		ous.close();
	}

	void setStateNum(int state_num) {
		//called by Gmacs
		_state_num = state_num;
	}
	
	void go() {
		//if (state_num == cur_state_num) return;
		for (int i = 0; i < state_num; i++) {
			EventState state = states[i];
			global_event_func = state.func;
			dynamic[] args = state.args;
			foreach (dynamic arg in args) {
				global_args << arg;
			}
			String eventfunc_definition = state.funcType + " event_func;";
			eval(eventfunc_definition);
			String assign_func_ptr_to_eventfunc = "event_func = (" + state.funcType + ")global_event_func;";
			eval(assign_func_ptr_to_eventfunc);
			String eval_script = "event_func(";
			if (args.size > 0) {
				String cast = "(" + state.types[0] + ")";
				eval_script += cast + "global_args[0]";
				for (int j = 1; j < args.size; j++) {
					eval_script += args[j] + ", ";
				}
			}
			eval_script += ");";
			print eval_script;
			eval(eval_script);
		}
	}
}
