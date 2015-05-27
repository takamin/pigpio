struct transtate_t;
struct statemac_t;
typedef struct transtate_t {
    struct transtate_t* base;
    void (* enter)(struct transtate_t* transtate);
    void (* run)(struct transtate_t* transtate);
    void (* leave)(struct transtate_t* transtate);
} TRANSTATE;
typedef struct statemac_t {
    TRANSTATE* states;
    int states_count;
    TRANSTATE* current_state;
    TRANSTATE* next_state;
} STATEMAC;

int statemac_init(STATEMAC* statemac, TRANSTATE* transtates, int count);
int statemac_run(STATEMAC* statemac);
int statemac_transition_to(STATEMAC* statemac, TRANSTATE* next_state);
