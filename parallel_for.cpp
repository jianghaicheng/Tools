#include "oneapi/tbb/blocked_range.h"
#include "oneapi/tbb/parallel_for.h"
#include <oneapi/tbb/task_arena.h>

+#include <oneapi/tbb/concurrent_unordered_map.h>

 using CodeSizeSparseVec = tbb::concurrent_unordered_map<int, uint64_t>;
std::vector<std::thread> allThreads;
std::vector<std::shared_ptr<pva::Program>> allPrograms(programs().size());
std::copy(programs().begin(),programs().end(),allPrograms.begin());

oneapi::tbb::task_arena arena(30);
arena.execute([&]{
oneapi::tbb::parallel_for(tbb::blocked_range<size_t>(0, allPrograms.size()),
  [&](const tbb::blocked_range<size_t>& range) {
     for (size_t i=range.begin();i<range.end();++i){
       allPrograms[i]->accept(visitor);
     } 
   });
 });


    // Limit the number of threads to two for all oneTBB parallel interfaces
    oneapi::tbb::global_control global_limit(oneapi::tbb::global_control::max_allowed_parallelism, 2);

    // the default parallelism
    oneapi::tbb::parallel_for( /* ... */ [] {
        // No more than two threads is expected; however, tbb::this_task_arena::max_concurrency() can return a bigger value
        int thread_limit = oneapi::tbb::global_control::active_value(oneapi::tbb::global_control::max_allowed_parallelism);
        assert(thread_limit == 2);
    });
