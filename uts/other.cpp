libnovel::Module* m = new libnovel::Module( "func_module" );
libnovel::BitConstant* c = libnovel::BitConstant::create( false, 1 );

libnovel::Function* f = new libnovel::Function( "func" );
libnovel::Scope* s = new libnovel::ParallelScope( f );
libnovel::Statement* tr = new libnovel::TrivialStatement( s );
libnovel::NotInstruction* ti = new libnovel::NotInstruction( c );
tr->add( ti );

// libnovel::Statement* b = new libnovel::BranchStatement( s );
// // libnovel::NotInstruction* i_ = new libnovel::NotInstruction( c );
// // libnovel::NotInstruction* i = new libnovel::NotInstruction( i_ );
// libnovel::NotInstruction* i = new libnovel::NotInstruction( c );
// b->add( i );

// libnovel::Scope* bt = new libnovel::SequentialScope();
// // libnovel::Scope* bf = new libnovel::SequentialScope();
// b->addScope( bt );
// // b->addScope( bf );

// libnovel::Statement* t0 = new libnovel::TrivialStatement( bt );
// t0->add( new libnovel::NopInstruction() );
// libnovel::Statement* t01 = new libnovel::TrivialStatement( bt );
// t01->add( new libnovel::NopInstruction() );

// // libnovel::Statement* t1 = new libnovel::TrivialStatement( bf );
// // t1->add( new libnovel::NopInstruction() );
// // libnovel::Statement* t11 = new libnovel::TrivialStatement( bf );
// // t11->add( new libnovel::NopInstruction() );

libnovel::Statement* loop = new libnovel::LoopStatement( s );
libnovel::NotInstruction* i = new libnovel::NotInstruction( c );
loop->add( i );

libnovel::Scope* body = new libnovel::SequentialScope();
loop->addScope( body );
libnovel::Statement* body0 = new libnovel::TrivialStatement( body );
body0->add( new libnovel::NopInstruction() );

m->add( f );
m->add( c );

value = m;
module = m;
