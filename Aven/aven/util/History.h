#pragma once

#include <list>


namespace aven {
	// history always contains at least 2 entries
	// back() is working entry

	template<typename T>
	class History {
	public:
		History(T scene):
			max_undos(16) 
		{
			history.push_back(scene);
			commit();
		}

		T& getCurrent() {
			assert(history.size() > 0);
			return history.back();
		}

		T const& getCurrent() const{
			assert(history.size() > 0);
			return history.back();
		}

		void commit() {
			assert(history.size() > 0);
			future.clear();
			history.push_back(history.back());
			if (getUndoCount() > max_undos)
				history.pop_front();
		}

		void cancelCurrent() {
			assert(history.size() >= 2);
			history.pop_back();
			history.push_back(history.back());
		}


		size_t getUndoCount()	{ return history.size()-2;		}
		bool undoable()		{ return history.size() > 2;	}
		bool redoable()		{ return future.size() > 0;		}


		bool undo() {
			if (!undoable())
				return false;
			history.pop_back();
			future.push_back(std::move(history.back()));
			history.pop_back();
			history.push_back(history.back());
			return true;
		}


		bool redo() {
			if (!redoable())
				return false;

			assert(history.size() >= 2);
			history.pop_back();
			history.push_back(std::move(future.back()));
			future.pop_back();
			history.push_back(history.back());
			return true;
		}


	private:
		// History contains always at least 2 entries: back() is entry currently being worked on
		std::list<T>	history;
		std::list<T>	future;

		int max_undos;
	};

}