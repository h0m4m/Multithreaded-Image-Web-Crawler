# Multithreaded Web Crawler for Image Retrieval

Welcome to the Multithreaded Web Crawler for Image Retrieval! This program is designed to install images from multiple URLs provided by the user in a Command Line Interface (CLI) format. The application prompts the user for different URLs and then initializes threads for each URL, subsequently crawling and retrieving all images present in them.

## Features

- **Multithreaded Crawling**: Utilize multiple threads to concurrently crawl and retrieve images from different URLs, optimizing performance and speed.
- **CLI Interface**: Simple and interactive Command Line Interface for user input and navigation.
- **Image Retrieval**: The program systematically extracts and installs images found within the provided URLs.
- **Flexible and Extensible**: Designed for flexibility, allowing for future enhancements and modifications.

## Installation

program should be ran on a linux environemnt with the following dependancies installed:
```linux
sudo apt-get install libcurl4-openssl-dev
```
## Usage

1. Compile the file:

   ```linux
   g++ filename.cpp -lcurl -lpthread
   ```
2. Run the file:
   ```linux
   ./a.out
   ```

2. Follow the CLI prompts to input different URLs you want to crawl for images.
3. The program will initiate threads for each URL and retrieve the images found within them.


## Acknowledgements

- Developers and contributors who have contributed to open-source web crawling libraries and tools.
- Python community for providing a robust environment for multithreaded programming.
- Users and testers for providing valuable feedback to improve this tool.
