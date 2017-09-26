#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "../includes/circular_buffer.hpp"

TEST_CASE( "Buffer should be empty when initialized", "[CircularBuffer]" ) {
  CircularBuffer<uint32_t, 4> buffer;
  REQUIRE(buffer.Size() == 0);
  REQUIRE(buffer.Empty() == true);
}

TEST_CASE( "MaxSize should be specified", "[CircularBuffer]" ) {
  CircularBuffer<uint32_t, 10> buffer;
  REQUIRE(buffer.MaxSize() == 10);
}

TEST_CASE( "Popping when empty should return false", "[CircularBuffer]" ) {
  CircularBuffer<uint32_t, 10> buffer;
  uint32_t t;
  REQUIRE_FALSE(buffer.Pop(t));
  REQUIRE_FALSE(buffer.Pop(t));
}

TEST_CASE( "Pushing when in Full state should return false", "[CircularBuffer]" ) {
  CircularBuffer<uint32_t, 1> buffer;
  REQUIRE(buffer.Push(1342));
  REQUIRE_FALSE(buffer.Push(1342));
}

TEST_CASE( "Size should be equal to the total number of pushed elements", "[CircularBuffer]" ) {
  CircularBuffer<uint32_t, 10> buffer;
  REQUIRE(buffer.Size() == 0);

  REQUIRE(buffer.Push(1));
  REQUIRE(buffer.Size() == 1);

  REQUIRE(buffer.Push(2));
  REQUIRE(buffer.Size() == 2);

  REQUIRE(buffer.Push(3));
  REQUIRE(buffer.Size() == 3);

  REQUIRE(buffer.Push(4));
  REQUIRE(buffer.Size() == 4);
}

TEST_CASE( "Size should be equal to the total number pushed elements minus the drop ones", "[CircularBuffer]" ) {
  CircularBuffer<uint32_t, 10> buffer;
  REQUIRE(buffer.Size() == 0);
  uint32_t t;

  REQUIRE(buffer.Push(1));
  REQUIRE(buffer.Push(2));
  REQUIRE(buffer.Size() == 2);

  REQUIRE(buffer.Pop(t));
  REQUIRE(buffer.Size() == 1);

  REQUIRE(buffer.Push(4));
  REQUIRE(buffer.Size() == 2);

  REQUIRE(buffer.Pop(t));
  REQUIRE(buffer.Size() == 1);
}

TEST_CASE( "Access the pushed data", "[CircularBuffer]" ) {
  CircularBuffer<uint32_t, 10> buffer;

  buffer.Push(100);
  buffer.Push(200);
  buffer.Push(300);
  buffer.Push(400);

  uint32_t t;
  REQUIRE(buffer.Size() == 4);

  REQUIRE(buffer.Peek(0, t));
  REQUIRE(t == 100);

  REQUIRE(buffer.Peek(1, t));
  REQUIRE(t == 200);

  REQUIRE(buffer.Peek(2, t));
  REQUIRE(t == 300);

  REQUIRE(buffer.Peek(3, t));
  REQUIRE(t == 400);

  REQUIRE(buffer.Size() == 4);
}

TEST_CASE( "Buffer should be full", "[CircularBuffer]" ) {
  CircularBuffer<uint32_t, 4> buffer;
  REQUIRE(buffer.Size() == 0);

  REQUIRE(buffer.Push(100));
  REQUIRE(buffer.Push(200));
  REQUIRE(buffer.Push(300));
  REQUIRE(buffer.Push(400));

  REQUIRE(buffer.Size() == 4);
  REQUIRE(buffer.Full());
  REQUIRE_FALSE(buffer.Push(500));

  uint32_t t;
  REQUIRE(buffer.Pop(t));
  REQUIRE(buffer.Pop(t));
  REQUIRE(buffer.Pop(t));

  REQUIRE(buffer.Size() == 1);
  REQUIRE(buffer.Full() == false);
}


TEST_CASE( "Buffer should be empty after pushing and then dropping all of elements", "[CircularBuffer]" ) {
  CircularBuffer<uint32_t, 4> buffer;
  uint32_t t;
  REQUIRE(buffer.Push(100));
  REQUIRE(buffer.Pop(t));

  REQUIRE(buffer.Push(200));
  REQUIRE(buffer.Pop(t));
  REQUIRE(buffer.Push(300));
  REQUIRE(buffer.Push(400));
  REQUIRE(buffer.Pop(t));
  REQUIRE(buffer.Pop(t));
  REQUIRE(buffer.Empty());
}

TEST_CASE( "Popping elements should be in FIFO order", "[CircularBuffer]" ) {
  CircularBuffer<uint32_t, 4> buffer;
  REQUIRE(buffer.Push(101));
  REQUIRE(buffer.Push(202));
  REQUIRE(buffer.Push(303));
  REQUIRE(buffer.Push(404));

  uint32_t t;
  REQUIRE(buffer.Pop(t));
  REQUIRE(t == 101);

  REQUIRE(buffer.Pop(t));
  REQUIRE(t == 202);

  REQUIRE(buffer.Pop(t));
  REQUIRE(t == 303);

  REQUIRE(buffer.Pop(t));
  REQUIRE(t == 404);
}

TEST_CASE( "Test the circular pop/push", "[CircularBuffer]" ) {
  CircularBuffer<uint32_t, 4> buffer;
  REQUIRE(buffer.Size() == 0);
  REQUIRE(buffer.Push(101));
  REQUIRE(buffer.Push(202));
  REQUIRE(buffer.Push(303));
  REQUIRE(buffer.Push(404));
  //[101, 202, 303, 404]

  REQUIRE(buffer.Size() == 4);
  uint32_t t;

  //[t=202, 303, 404]
  REQUIRE(buffer.Pop(t));

  REQUIRE(buffer.Pop(t));

  //[t=303, 404]
  REQUIRE(buffer.Pop(t));
  REQUIRE(t == 303);

  REQUIRE(buffer.Size() == 1);

  REQUIRE(buffer.Push(71));
  REQUIRE(buffer.Size() == 2);
  //buffer => [404, 71]

  REQUIRE(buffer.Push(32));
  REQUIRE(buffer.Size() == 3);
  //buffer => [404, 71, 32]
  REQUIRE_FALSE(buffer.Full());

  REQUIRE(buffer.Push(43));
  //buffer => [404, 71, 32, 43]

  REQUIRE(buffer.Full());
  REQUIRE(buffer.Size() == 4);

  REQUIRE(buffer.Peek(0, t));
  REQUIRE(t == 404);

  REQUIRE(buffer.Peek(1, t));
  REQUIRE(t == 71);

  REQUIRE(buffer.Peek(2, t));
  REQUIRE(t == 32);

  REQUIRE(buffer.Peek(3, t));
  REQUIRE(t == 43);

  REQUIRE(buffer.Pop(t));
  REQUIRE(buffer.Pop(t));
  REQUIRE(buffer.Pop(t));
  REQUIRE(buffer.Pop(t));
  REQUIRE_FALSE(buffer.Pop(t));
  REQUIRE(buffer.Size() == 0);
}

