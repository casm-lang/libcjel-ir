libcsel_ir::Module* m = new libcsel_ir::Module( "func_module" );
libcsel_ir::BitConstant* c = libcsel_ir::BitConstant::create( false, 1 );

libcsel_ir::Function* f = new libcsel_ir::Function( "func" );
libcsel_ir::Scope* s = new libcsel_ir::ParallelScope( f );
libcsel_ir::Statement* tr = new libcsel_ir::TrivialStatement( s );
libcsel_ir::NotInstruction* ti = new libcsel_ir::NotInstruction( c );
tr->add( ti );

// libcsel_ir::Statement* b = new libcsel_ir::BranchStatement( s );
// // libcsel_ir::NotInstruction* i_ = new libcsel_ir::NotInstruction( c );
// // libcsel_ir::NotInstruction* i = new libcsel_ir::NotInstruction( i_ );
// libcsel_ir::NotInstruction* i = new libcsel_ir::NotInstruction( c );
// b->add( i );

// libcsel_ir::Scope* bt = new libcsel_ir::SequentialScope();
// // libcsel_ir::Scope* bf = new libcsel_ir::SequentialScope();
// b->addScope( bt );
// // b->addScope( bf );

// libcsel_ir::Statement* t0 = new libcsel_ir::TrivialStatement( bt );
// t0->add( new libcsel_ir::NopInstruction() );
// libcsel_ir::Statement* t01 = new libcsel_ir::TrivialStatement( bt );
// t01->add( new libcsel_ir::NopInstruction() );

// // libcsel_ir::Statement* t1 = new libcsel_ir::TrivialStatement( bf );
// // t1->add( new libcsel_ir::NopInstruction() );
// // libcsel_ir::Statement* t11 = new libcsel_ir::TrivialStatement( bf );
// // t11->add( new libcsel_ir::NopInstruction() );

libcsel_ir::Statement* loop = new libcsel_ir::LoopStatement( s );
libcsel_ir::NotInstruction* i = new libcsel_ir::NotInstruction( c );
loop->add( i );

libcsel_ir::Scope* body = new libcsel_ir::SequentialScope();
loop->addScope( body );
libcsel_ir::Statement* body0 = new libcsel_ir::TrivialStatement( body );
body0->add( new libcsel_ir::NopInstruction() );

m->add( f );
m->add( c );

value = m;
module = m;
