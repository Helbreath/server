//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include <filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <functional>
#include <string>

enum class file_status
{
    created,
    modified,
    deleted
};

class filewatcher
{
public:
    std::vector<std::string> paths_to_watch;
    std::chrono::duration<int32_t, std::milli> delay;

    filewatcher(std::vector<std::string> paths_to_watch, std::chrono::duration<int, std::milli> delay)
        : paths_to_watch{ paths_to_watch }, delay{ delay }
    {
        for (auto & path : paths_to_watch)
        {
            paths_[path] = std::filesystem::last_write_time(path);
        }
    }

    void add_file(std::string path)
    {
        if (!std::filesystem::exists(path))
            throw std::runtime_error(fmt::format("File [{}] does not exist", path));

        paths_to_watch.push_back(path);
        paths_[path] = std::filesystem::last_write_time(path);
    }

    void stop()
    {
        running_ = false;
    }

    void start(const std::function<void(std::string, file_status)> & action)
    {
        if (thread_ == nullptr)
        {
            thread_ = std::make_unique<std::thread>([&]()
            {
                while (running_)
                {
                    std::this_thread::sleep_for(delay);

                    auto it = paths_.begin();
                    while (it != paths_.end())
                    {
                        auto & path = it->first;
                        auto & time = it->second;

                        if (!std::filesystem::exists(path))
                        {
                            action(path, file_status::deleted);
                            it = paths_.erase(it);
                        }
                        else
                        {
                            it++;
                        }
                    }

                    for (auto & path : paths_to_watch)
                    {
                        auto current_file_last_write_time = std::filesystem::last_write_time(path);

                        if (!paths_.contains(path))
                        {
                            paths_[path] = current_file_last_write_time;
                            action(path, file_status::created);
                        }
                        else
                        {
                            if (paths_[path] != current_file_last_write_time)
                            {
                                paths_[path] = current_file_last_write_time;
                                action(path, file_status::modified);
                            }
                        }
                    }
                }
            });
        }
    }

private:
    std::unordered_map<std::string, std::filesystem::file_time_type> paths_;
    bool running_ = true;
    std::unique_ptr<std::thread> thread_;
};