#ifndef ABSOLUTE_CAST_H_
#define ABSOLUTE_CAST_H_

template <typename Dest, typename Src>
inline Dest& absolute_cast(Src &v) {
	return reinterpret_cast<Dest&>(v);
}

#endif /* ABSOLUTE_CAST_H_ */
