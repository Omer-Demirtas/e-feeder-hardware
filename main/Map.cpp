#include "Map.hpp";
#include "Arduino.h";


template <typename T, typename U> Map<T, U>::Map(int mSize) {
  T arrKeys[mSize];
  U arrValues[mSize];

  keys = arrKeys;
  values = arrValues;
  //keys = (T*)malloc(mSize * sizeof(T));
  //values = (U*)malloc(mSize * sizeof(U));
  mapSize=mSize;
}  

template <typename T, typename U> void Map<T, U>::append(T key, U value)
{
  if (size == mapSize) return;
  size++;
  keys[size] = key;
  values[size] = value;
}

template <typename T, typename U> U Map<T, U>::get(T key)
{
  U u;
  for (int i = 0; i < size; i++)
  {
    if(keys[i] == key)
    {
      return values[i];
    }
  }
  return u;
}

template <typename T, typename U> U Map<T, U>::remove(T key)
{ }

template <typename T, typename U> void Map<T, U>::print()
{
  Serial.println("***************");
  for(int i = 0; i < size; i++)
  {
    Serial.println(keys[i]);
    Serial.print(values[i]);    
  }
}

template class Map<String, int>;
template class Map<String, String>;
