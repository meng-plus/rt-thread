#include "EfsmTaskCy.hpp"
#include "EfsmNodeIdle.hpp"
#include "EfsmNodeWarning.hpp"
CEfsmTaskCy::CEfsmTaskCy(/* args */)
{
    // m_nodeArr.reserve(7);
    m_nodeArr.resize(7);

    m_nodeArr[0] = new CEfsmNodeIdle();
    m_nodeArr[6] = new CEfsmNodeStep1();
}

CEfsmTaskCy::~CEfsmTaskCy()
{
}

void CEfsmTaskCy::action(const CEfsmEvent *Event_ptr)
{
    if (Event_ptr == NULL)
    {
        return;
    }

    switch (Event_ptr->m_event)
    {
    case EFSM_EVENT::IDLE:
        break;
    case EFSM_EVENT::TICK:
        if(m_nodeArr[m_idx])
          m_nodeArr[m_idx]->tick();
        break;
    case EFSM_EVENT::START:
        break;
    case EFSM_EVENT::STOP:
        break;
    case EFSM_EVENT::PREV:
        break;
    case EFSM_EVENT::NEXT:
        break;
    case EFSM_EVENT::WARNING:
        break;
    case EFSM_EVENT::ERROR:
        break;
    case EFSM_EVENT::VALUE:
        break;
    case EFSM_EVENT::USER:
        break;
    default:
        break;
    }
}

uint8_t CEfsmTaskCy::getDir()
{
    return m_dir;
}

void CEfsmTaskCy::setDir(uint8_t dir)
{
    if (m_dir_MAX > dir)
    {
        m_dir = dir;
    }
}
