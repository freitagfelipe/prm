Write-Output "Installing PRM..."

try {
    if (-Not (Get-Command cmake)) {
        throw "You need CMake to build PRM"
    } elseif (-Not (Get-Command make)) {
        throw "You need make to build PRM"
    }

    cd ..
    mkdir build
    cd build
    cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
    cmake --build .

    $destination = "$env:ProgramFiles/prm"

    if (-Not (Test-path $destination)) {
        Write-Output "Creating a folder called prm to store the executable in: $env:ProgramFiles"

        New-item -ItemType Directory -Path $destination
    }

    echo "Moving the PRM executable to the $destination"

    Move-Item -Path ".\prm.exe" -Destination $destination -ErrorAction Stop

    echo "Adding the $destination to the system PATH"

    $env:PATH += ";$destination"

    Write-Output "Finished the installation"
} catch {
    Write-Output $_.Exception.Message
}