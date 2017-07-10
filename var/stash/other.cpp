libcjel_ir::Module* m = new libcjel_ir::Module( "func_module" );
libcjel_ir::BitConstant* c = libcjel_ir::BitConstant::create( false, 1 );

libcjel_ir::Function* f = new libcjel_ir::Function( "func" );
libcjel_ir::Scope* s = new libcjel_ir::ParallelScope( f );
libcjel_ir::Statement* tr = new libcjel_ir::TrivialStatement( s );
libcjel_ir::NotInstruction* ti = new libcjel_ir::NotInstruction( c );
tr->add( ti );

// libcjel_ir::Statement* b = new libcjel_ir::BranchStatement( s );
// // libcjel_ir::NotInstruction* i_ = new libcjel_ir::NotInstruction( c );
// // libcjel_ir::NotInstruction* i = new libcjel_ir::NotInstruction( i_ );
// libcjel_ir::NotInstruction* i = new libcjel_ir::NotInstruction( c );
// b->add( i );

// libcjel_ir::Scope* bt = new libcjel_ir::SequentialScope();
// // libcjel_ir::Scope* bf = new libcjel_ir::SequentialScope();
// b->addScope( bt );
// // b->addScope( bf );

// libcjel_ir::Statement* t0 = new libcjel_ir::TrivialStatement( bt );
// t0->add( new libcjel_ir::NopInstruction() );
// libcjel_ir::Statement* t01 = new libcjel_ir::TrivialStatement( bt );
// t01->add( new libcjel_ir::NopInstruction() );

// // libcjel_ir::Statement* t1 = new libcjel_ir::TrivialStatement( bf );
// // t1->add( new libcjel_ir::NopInstruction() );
// // libcjel_ir::Statement* t11 = new libcjel_ir::TrivialStatement( bf );
// // t11->add( new libcjel_ir::NopInstruction() );

libcjel_ir::Statement* loop = new libcjel_ir::LoopStatement( s );
libcjel_ir::NotInstruction* i = new libcjel_ir::NotInstruction( c );
loop->add( i );

libcjel_ir::Scope* body = new libcjel_ir::SequentialScope();
loop->addScope( body );
libcjel_ir::Statement* body0 = new libcjel_ir::TrivialStatement( body );
body0->add( new libcjel_ir::NopInstruction() );

m->add( f );
m->add( c );

value = m;
module = m;
