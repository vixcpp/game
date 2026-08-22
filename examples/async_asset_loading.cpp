#include <vix/game/game.hpp>
#include <vix/print.hpp>

int main()
{
  vix::game::AssetManager assets("assets");
  vix::game::JobSystem jobs;
  vix::game::RuntimeDispatcher dispatcher;
  dispatcher.bind_current_thread();
  vix::game::AsyncAssetLoader loader(assets, jobs, dispatcher);

  auto handle = loader.load(
      "example.txt",
      [](vix::game::GameResult<vix::game::AssetId> result)
      {
        if (!result)
        {
          vix::print("async asset failed:", result.error().message());
          return;
        }

        vix::print("async asset loaded:", result.value());
      });

  if (!handle)
  {
    vix::print("failed to start async load:", handle.error().message());
    return 1;
  }

  handle.value().wait();
  handle.value().get();

  auto drained = dispatcher.drain();
  if (!drained)
  {
    vix::print("failed to process async completion:", drained.error().message());
    jobs.shutdown();
    return 1;
  }

  jobs.shutdown();

  return 0;
}
