void MemoryTimelineSystem::pruneOldSnapshots() {
    const size_t maxStorageBytes = 100 * 1024 * 1024;  // 100MB default
    size_t currentStorageSize = 0;
    
    // Calculate current storage usage
    for (const auto& snapshot : m_History) {
        currentStorageSize += snapshot.getStorageSize();
    }
    
    // Remove old snapshots if exceeding limit
    while (!m_History.empty() && currentStorageSize > maxStorageSize) {
        currentStorageSize -= m_History.front().getStorageSize();
        m_History.pop_front();
    }
    
    // Clean up cached files older than 24 hours
    cleanupCachedFiles();
}

void MemoryTimelineSystem::cleanupCachedFiles() {
    QDir cacheDir(getCacheDirectory());
    auto files = cacheDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    
    QDateTime cutoff = QDateTime::currentDateTime().addDays(-1);
    for (const auto& file : files) {
        if (file.lastModified() < cutoff) {
            QFile::remove(file.absoluteFilePath());
        }
    }
} 