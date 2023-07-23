
namespace ut::game::sys
{

template <typename Cond>
auto EntityManager::findIf(Cond condition, decltype(_entities)::iterator prevIt) -> decltype(_entities)::iterator
{
    if (prevIt == _entities.end())
        return _entities.end();

    auto it = ++prevIt;
    while (it != _entities.end() && !condition(*it))
        ++it;

    return it;
}

template <typename Cond>
auto EntityManager::findIf(Cond condition, decltype(_entities)::const_iterator prevIt) const
    -> decltype(_entities)::const_iterator
{
    if (prevIt == _entities.cend())
        return _entities.cend();

    auto it = ++prevIt;
    while (it != _entities.cend() && !condition(*it))
        ++it;

    return it;
}

} // namespace ut::game::sys
