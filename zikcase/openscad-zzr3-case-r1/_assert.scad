// found at:
// https://github.com/openscad/openscad/issues/381

// Called when an assertion fails (invokes infinite recursion).
function assertion_failed() = (assertion_failed());

// Assert that `bool` is true.
//
// If `bool` is false, emit the error `msg`,
//  and terminate compilation (via infinite recursion).
module assert(bool, msg = "")
{
    if(bool == false)
    {
        echo("Assertion Failed: ", msg);
        echo("", assertion_failed());
    }
}
